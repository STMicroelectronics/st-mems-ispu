/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "motion_sd.h"

#define ACC_SENS 0.000488f
#define PRS_SENS 0.000244140625f

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

void __attribute__ ((signal)) algo_00_init(void)
{
	MotionSD_Initialize();
	MotionSD_SetOrientation_Acc("enu");
}

void __attribute__ ((signal)) algo_00(void)
{
	MSD_input_t in;
	MSD_output_t out = MSD_UNKNOWN_DESK;

	in.AccX = (float)cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	in.AccY = (float)cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	in.AccZ = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	uint32_t prs_raw = (uint32_t)cast_uint16_t(ISPU_ERAW_0) + ((uint32_t)cast_uint8_t(ISPU_ERAW_1) << 16);
	in.Press = (float)prs_raw * PRS_SENS;

	MotionSD_Update(&in, &out);

	cast_float(ISPU_DOUT_00) = in.AccX;
	cast_float(ISPU_DOUT_02) = in.AccY;
	cast_float(ISPU_DOUT_04) = in.AccZ;
	cast_float(ISPU_DOUT_06) = in.Press;
	cast_uint8_t(ISPU_DOUT_08) = (uint8_t)out;

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

