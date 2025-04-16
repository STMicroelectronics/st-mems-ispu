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

#include "stai.h"
#include "network.h"
#include "network_utils.h"
#include "iir2.h"
#include "ispu_wand_params.h"

enum {
	STATE_READY,
	STATE_UPDATE,
	STATE_RESET
};

struct iir2 filters[NUM_AXES];
static uint8_t algo_state;
static uint16_t trig_cnt;
static uint16_t win_cnt;
static uint16_t reset_cnt;
static uint8_t prediction;

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static __attribute__((aligned(8))) stai_network net[STAI_NETWORK_CONTEXT_SIZE];

static stai_ptr input_buffers[STAI_NETWORK_IN_NUM];
static stai_ptr output_buffers[STAI_NETWORK_OUT_NUM];

static volatile uint32_t int_status;

uint8_t run_inference(float *output)
{
	uint8_t pred = LABEL_NULL;
	stai_return_code res = stai_network_run(net, STAI_MODE_SYNC);
	if (res == STAI_SUCCESS) {
		float max_prob = -1.0f;
		for (uint8_t i = 0; i < NUM_LABELS; i++) {
			if (output[i] > max_prob) {
				max_prob = output[i];
				pred = i;
			}
		}
		if (max_prob < PRED_THS) {
			pred = LABEL_NULL;
		}
	}

	return pred;
}

void __attribute__ ((signal)) algo_00_init(void)
{
	(void)stai_runtime_init();
	(void)stai_network_init(net); // initialize the network context

	init_network_buffers(net, input_buffers, output_buffers);

	// initialize filters
	struct iir2_conf filter_conf = {
		.b = IIR2_B,
		.a = IIR2_A,
		.fast_set = true,
	};
	for (uint8_t i = 0; i < NUM_AXES; i++) {
		iir2_init(&(filters[i]), &filter_conf);
	}

	// initialize state logic variables
	prediction = LABEL_NULL;
	algo_state = STATE_RESET;
	trig_cnt = 0;
	win_cnt = 0;
	reset_cnt = 0;
}

void __attribute__ ((signal)) algo_00(void)
{
	float raw_data[NUM_AXES];
	float filt_data[NUM_AXES];

	// CNN input and output pointers
	float (*input)[WIN_LEN_SAMPLES][3] = (float (*)[WIN_LEN_SAMPLES][3])input_buffers[0];
	float (*output)[NUM_LABELS] = (float (*)[NUM_LABELS])output_buffers[0];

	// apply bandpass filters on acc_x, acc_y, acc_z
	raw_data[0] = cast_sint16_t(ISPU_ARAW_X) * ACC_SENS_G;
	filt_data[0] = iir2_run(&(filters[0]), raw_data[0]);

	raw_data[1] = cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS_G;
	filt_data[1] = iir2_run(&(filters[1]), raw_data[1]);

	raw_data[2] = cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS_G;
	filt_data[2] = iir2_run(&(filters[2]), raw_data[2]);

	switch (algo_state) {
	// wait for start trigger
	case STATE_READY:
		if (filt_data[0] > WIN_THS_G) {
			trig_cnt++;
		} else {
			trig_cnt = 0;
		}
		if (trig_cnt == TRIG_LEN_SAMPLES) {
			trig_cnt = 0;
			algo_state = STATE_UPDATE;
		}
		break;

	// update input buffer
	case STATE_UPDATE:
		for (uint8_t ch = 0; ch < NUM_AXES; ch++) {
			input[0][win_cnt][ch] = filt_data[ch];
		}
		win_cnt++;
		if (win_cnt == WIN_LEN_SAMPLES) {
			win_cnt = 0;
			prediction = run_inference(output[0]);
			algo_state = STATE_RESET;
		}
		break;

	// wait for reset samples
	case STATE_RESET:
		reset_cnt++;
		if (reset_cnt == RESET_LEN_SAMPLES) {
			reset_cnt = 0;
			algo_state = STATE_READY;
		}
		break;
	}

	// write results to output registers
	uint32_t addr = ISPU_DOUT_00;
	for (uint8_t i = 0; i < NUM_AXES; i++) {
		cast_float(addr) = raw_data[i];
		addr += sizeof(float);
	}

	cast_uint8_t(addr) = algo_state == STATE_UPDATE ? (uint8_t)1 : (uint8_t)0;
	addr += sizeof(uint8_t);
	for (uint8_t i = 0; i < NUM_LABELS; i++) {
		cast_float(addr) = output[0][i];
		addr += sizeof(float);
	}
	cast_char(addr) = LABELS[prediction];
	addr += sizeof(char);

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

