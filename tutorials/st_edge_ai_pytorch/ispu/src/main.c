/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "peripherals.h"
#include "reg_map.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stai.h"
#include "network.h"
#include "network_utils.h"

#define ACC_SENS 0.244f // [mg/LSB]
#define ACC_NUM_AXES 3
#define WIN_LEN 52

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static void reset_status(void);
static inline float signf(float value);
static void compute_mcr(void);

static __attribute__((aligned(8))) stai_network net[STAI_NETWORK_CONTEXT_SIZE];

static stai_ptr input_buffers[STAI_NETWORK_IN_NUM];
static stai_ptr output_buffers[STAI_NETWORK_OUT_NUM];

static volatile uint32_t int_status;

static float mean[ACC_NUM_AXES];
static float var[ACC_NUM_AXES];
static uint8_t mcr[ACC_NUM_AXES];
static float signal_buf[WIN_LEN][ACC_NUM_AXES];

static uint8_t win_cnt;

// standard scaler parameters
static const float features_mean[STAI_NETWORK_IN_1_CHANNEL] = {
	-489.4554374956601f,
	536.8898007211596f,
	315.4528401601346f,
	516806.8996868161f,
	992323.5776839033f,
	86117.80626196241f,
	15.409532215357459f,
	17.17828773168579f,
	16.784642541924097f
};

static const float features_inv_std_dev[STAI_NETWORK_IN_1_CHANNEL] = {
	0.002183945503568183f,
	0.001915411784024902f,
	0.0026443263488319492f,
	1.026182957270523e-06f,
	4.857604595408713e-07f,
	6.19934087047198e-06f,
	0.09091009321406888f,
	0.104295242626417f,
	0.1065995815529936f
};

void __attribute__ ((signal)) algo_00_init(void)
{
	(void)stai_runtime_init();
	(void)stai_network_init(net); // initialize the network context

	init_network_buffers(net, input_buffers, output_buffers);

	// initialize state variables and buffers
	reset_status();
}

static void reset_status(void)
{
	for (uint8_t ax = 0; ax < ACC_NUM_AXES; ax++) {
		mean[ax] = 0.0f;
		var[ax] = 0.0f;
		mcr[ax] = 0;
	}
	win_cnt = 0;
}

static inline float signf(float value)
{
	float sign = 0.0f;

	if (value > 0.0f) {
		sign = 1.0f;
	} else if (value < 0.0f) {
		sign = -1.0f;
	}

	return sign;
}

static void compute_mcr(void)
{
	float sign[ACC_NUM_AXES];

	for (uint8_t i = 0; i < WIN_LEN; i++) {
		for (uint8_t ax = 0; ax < ACC_NUM_AXES; ax++) {
			float sample_sign = signf(signal_buf[i][ax] - mean[ax]);

			if (i == 0) {
				sign[ax] = sample_sign;
			}

			if (sample_sign * sign[ax] < 0.0f) {
				sign[ax] = sample_sign;
				mcr[ax] += 1;
			}
		}
	}
}

void __attribute__ ((signal)) algo_00(void)
{
	// ispu sensor data registers base address
	void *in_addr = (void *)ISPU_ARAW_X;
	// ispu output registers base address
	void *out_addr = (void *)ISPU_DOUT_00;

	for (uint8_t ax = 0; ax < ACC_NUM_AXES; ax++) {
		// read accelerometer data
		float value = (float)cast_sint16_t(in_addr) * ACC_SENS;

		// compute mean and variance incrementally
		float delta = value - mean[ax];
		mean[ax] +=  delta / (win_cnt + 1);
		var[ax] += (value - mean[ax]) * delta;

		// accumulate the accelerometer data in the buffer for mean-crossing rate computation
		signal_buf[win_cnt][ax] = value;

		// write accelerometer data to output registers
		cast_float(out_addr) = value;
		out_addr += sizeof(float);

		in_addr += sizeof(int32_t);
	}

	// increment count and check if the end of the window is reached
	if (++win_cnt == WIN_LEN) {
		// complete features computation
		for (uint8_t ax = 0; ax < ACC_NUM_AXES; ax++) {
			var[ax] *= (1.0f / WIN_LEN);
		}
		compute_mcr();

		// write features to output registers
		memcpy(out_addr, mean, sizeof(mean));
		out_addr += sizeof(mean);

		memcpy(out_addr, var, sizeof(var));
		out_addr += sizeof(var);

		memcpy(out_addr, mcr, sizeof(mcr));
		out_addr += sizeof(mcr);

		// reinterpret input buffer as a multi-dimensional array of shape {1,9}
		float (*input)[STAI_NETWORK_IN_1_CHANNEL] = (float (*)[STAI_NETWORK_IN_1_CHANNEL])input_buffers[0];

		// reinterpret output buffer as a multi-dimensional array of shape {1,4}
		float (*output)[STAI_NETWORK_OUT_1_CHANNEL] = (float (*)[STAI_NETWORK_OUT_1_CHANNEL])output_buffers[0];

		// place the features inside the input buffer
		for (uint8_t ax = 0; ax < ACC_NUM_AXES; ax++) {
			input[0][ax] = mean[ax];
			input[0][ACC_NUM_AXES + ax] = var[ax];
			input[0][(ACC_NUM_AXES * 2) + ax] = (float)mcr[ax];
		}

		// normalize features before running the inference
		for (uint8_t i = 0; i < STAI_NETWORK_IN_1_CHANNEL; i++) {
			input[0][i] = (input[0][i] - features_mean[i]) * features_inv_std_dev[i];
		}

		// run model inference
		stai_network_run(net, STAI_MODE_SYNC);

		// prediction corresponds to the output with the highest probability
		uint8_t prediction = 0;
		float max_prob = -1.0f;
		for (uint8_t i = 0; i < STAI_NETWORK_OUT_1_CHANNEL; i++) {
			if (output[0][i] > max_prob) {
				max_prob = output[0][i];
				prediction = i;
			}
		}

		// write prediction results to output registers
		memcpy(out_addr, output[0], STAI_NETWORK_OUT_1_CHANNEL * sizeof(float));
		out_addr += STAI_NETWORK_OUT_1_CHANNEL * sizeof(float);
		cast_uint8_t(out_addr) = prediction;

		reset_status();
	}

	// interrupt generation
	int_status = int_status | 0x1u;
}

int main(void)
{
	// set boot done flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x04u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable algorithms interrupt request generation
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x01u;

	while (true) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0u;
		cast_uint32_t(ISPU_INT_STATUS) = 0u;
		cast_uint8_t(ISPU_INT_PIN) = 0u;

		// get all the algorithms to run in this time slot
		cast_uint32_t(ISPU_CALL_EN) = cast_uint32_t(ISPU_ALGO) << 1;

		// wait for all algorithms execution
		while (cast_uint32_t(ISPU_CALL_EN) != 0u) {
		}

		// get interrupt flags
		uint8_t int_pin = 0u;
		int_pin |= ((int_status & cast_uint32_t(ISPU_INT1_CTRL)) > 0u) ? 0x01u : 0x00u;
		int_pin |= ((int_status & cast_uint32_t(ISPU_INT2_CTRL)) > 0u) ? 0x02u : 0x00u;

		// set status registers and generate interrupts
		cast_uint32_t(ISPU_INT_STATUS) = int_status;
		cast_uint8_t(ISPU_INT_PIN) = int_pin;
	}
}

