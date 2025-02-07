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

#define ACC_ODR 26 // [Hz]
#define ACC_FS 8 // [g]
#define ACC_SENS 0.244 // [mg/LSB]

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static __attribute__((aligned(8))) stai_network net[STAI_NETWORK_CONTEXT_SIZE];

static stai_ptr input_buffers[STAI_NETWORK_IN_NUM];
static stai_ptr output_buffers[STAI_NETWORK_OUT_NUM];

static volatile uint32_t int_status;

static const char *labels[] = { "stationary", "walking", "running", "cycling" };
static uint8_t win_cnt;
static int8_t prediction;

void __attribute__ ((signal)) algo_00_init(void)
{
	(void)stai_runtime_init();
	(void)stai_network_init(net); // initialize the network context

	init_network_buffers(net, input_buffers, output_buffers);

	// initialize state variables
	win_cnt = 0;
	prediction = 0;
}

void __attribute__ ((signal)) algo_00(void)
{
	// ispu output registers base address
	uint32_t addr = ISPU_DOUT_00;

	// reinterpret input buffer as a multi-dimensional array of shape {1,52,3}
	float (*input)[STAI_NETWORK_IN_1_HEIGHT][STAI_NETWORK_IN_1_CHANNEL] =
		(float (*)[STAI_NETWORK_IN_1_HEIGHT][STAI_NETWORK_IN_1_CHANNEL])input_buffers[0];

	// reinterpret output buffer as a multi-dimensional array of shape {1,4}
	float (*output)[STAI_NETWORK_OUT_1_CHANNEL] =
		(float (*)[STAI_NETWORK_OUT_1_CHANNEL])output_buffers[0];

	// read accelerometer data and place it inside input buffer
	input[0][win_cnt][0] = cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	input[0][win_cnt][1] = cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	input[0][win_cnt][2] = cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;

	// write accelerometer data to output registers
	for (uint8_t i = 0; i < STAI_NETWORK_IN_1_CHANNEL; i++, addr += sizeof(float))
		cast_float(addr) = input[0][win_cnt][i];

	// increment count and check if input buffer is ready
	if (++win_cnt == STAI_NETWORK_IN_1_HEIGHT) {
		win_cnt = 0;

		// run model inference
		stai_network_run(net, STAI_MODE_SYNC);

		// prediction corresponds to the output with the highest probability
		float max_prob = -1.0f;
		for (uint8_t i = 0; i < STAI_NETWORK_OUT_1_CHANNEL; i++) {
			if (output[0][i] > max_prob) {
				max_prob = output[0][i];
				prediction = i;
			}
		}
	}

	// write prediction results to output registers
	for (uint8_t i = 0; i < STAI_NETWORK_OUT_1_CHANNEL; i++, addr += sizeof(float))
		cast_float(addr) = output[0][i];
	strcpy((char *)addr, labels[prediction]);

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

