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
#include <string.h>
#include "motion_aw.h"

#define ACC_SENS 0.000244f

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;
static int64_t timestamp;
static float dtime;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	(void)memcpy((void *)&dtime, (void *)&dt, sizeof(float));
	dtime *= 4000.0f;

	MotionAW_Initialize();
	MotionAW_SetOrientation_Acc("enu");

	timestamp = 0;
}

void __attribute__ ((signal)) algo_00(void)
{
	MAW_input_t in;
	MAW_output_t out;
	(void)memset(&out, 0, sizeof(out));

	in.AccX = (float)cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	in.AccY = (float)cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	in.AccZ = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	timestamp += (int)dtime;

	MotionAW_Update(&in, &out, timestamp);

	cast_float(ISPU_DOUT_00) = in.AccX;
	cast_float(ISPU_DOUT_02) = in.AccY;
	cast_float(ISPU_DOUT_04) = in.AccZ;
	cast_uint8_t(ISPU_DOUT_06) = (uint8_t)out.current_activity;
	cast_uint8_t(ISPU_DOUT_06_B) = (uint8_t)out.confidence;

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

