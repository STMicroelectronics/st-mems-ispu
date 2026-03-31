/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "ispu_float.h"

#include "dot.h"

#include <stdint.h>
#include <stdbool.h>

#define ACC_SENS 0.000095f

static volatile uint16_t int_status;

static volatile float acc[3];
static float acc_prev[3];

void __attribute__ ((signal)) data_valid_interrupt(void)
{
	acc[0] = sint20_to_float(cast_sint32_t(ISPU_ARAW_X)) * ACC_SENS;
	acc[1] = sint20_to_float(cast_sint32_t(ISPU_ARAW_Y)) * ACC_SENS;
	acc[2] = sint20_to_float(cast_sint32_t(ISPU_ARAW_Z)) * ACC_SENS;
}

void __attribute__ ((signal)) algo_00_init(void)
{
}

void __attribute__ ((signal)) algo_00(void)
{
	float res = dot((float *)acc, acc_prev, 3);

	cast_float(ISPU_DOUT_00) = acc[0];
	cast_float(ISPU_DOUT_02) = acc[1];
	cast_float(ISPU_DOUT_04) = acc[2];
	cast_float(ISPU_DOUT_06) = acc_prev[0];
	cast_float(ISPU_DOUT_08) = acc_prev[1];
	cast_float(ISPU_DOUT_10) = acc_prev[2];
	cast_float(ISPU_DOUT_12) = res;

	for (uint8_t i = 0; i < 3; i++)
		acc_prev[i] = acc[i];

	int_status = int_status | 0x1u;
}

int main(void)
{
	// set boot end flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x40u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable high-priority (data valid) interrupt and algorithms interrupt request generation
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x09;

	while (true) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0u;
		cast_uint16_t(ISPU_INT_STATUS) = 0u;
		cast_uint8_t(ISPU_INT_PIN) = 0u;

		// trigger enabled algorithms execution
		cast_uint32_t(ISPU_CALL_EN) = cast_uint16_t(ISPU_ALGO) << 4;

		// wait for all algorithms execution
		while (cast_uint32_t(ISPU_CALL_EN & 0xFFFF0) != 0u)
			;

		// get interrupt flags
		uint8_t int_pin = 0u;
		int_pin |= ((int_status & cast_uint16_t(ISPU_INT1_CTRL)) > 0u) ? 0x01u : 0x00u;
		int_pin |= ((int_status & cast_uint16_t(ISPU_INT2_CTRL)) > 0u) ? 0x02u : 0x00u;

		// set status registers and generate interrupts
		cast_uint16_t(ISPU_INT_STATUS) = int_status;
		cast_uint8_t(ISPU_INT_PIN) = int_pin;
	}
}

