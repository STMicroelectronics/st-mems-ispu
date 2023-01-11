/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include <stddef.h>
#include "motion_ft.h"

#define ACC_SENS 0.000244f
#define WIN_LEN 26u
#define DFT_LEN ((WIN_LEN / 2u) + 1u)

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

static void *mft;

void __attribute__ ((signal)) algo_00_init(void)
{
	MFT_conf_t conf = {
		.winlen = WIN_LEN,
		.damping = 1.0f,
		.normalize = 1u
	};

	if (mft != NULL) {
		MotionFT_deinitialize(mft);
	}
	mft = MotionFT_initialize(&conf);
}

void __attribute__ ((signal)) algo_00(void)
{
	MFT_input_t data_in;
	float dft_mag[DFT_LEN];
	MFT_output_t data_out = { .dft_mag = dft_mag };

	data_in.sample = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	MotionFT_update(mft, &data_out, &data_in);

	cast_float(ISPU_DOUT_00) = data_in.sample;
	for (uint16_t i = 0u; i < DFT_LEN; i++) {
		cast_float(ISPU_DOUT_02 + ((int16_t)i * 4)) = dft_mag[i];
	}

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

