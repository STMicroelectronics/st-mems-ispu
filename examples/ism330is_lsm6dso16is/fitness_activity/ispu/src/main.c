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
#include "motion_fa.h"

#define ACC_SENS 0.000488f
#define GYR_SENS 0.070f
#define PRS_SENS 0.000244140625f

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

static float dtime;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	(void)memcpy((void *)&dtime, (void *)&dt, sizeof(float));
	dtime *= 4.0f;

	MotionFA_initialize(MFA_LEFT);

	MotionFA_bicep_enable(MFA_ENABLE);
	MotionFA_squat_enable(MFA_ENABLE);
	MotionFA_push_up_enable(MFA_ENABLE);
	MotionFA_lateral_enable(MFA_ENABLE);
	MotionFA_overhead_enable(MFA_ENABLE);
	MotionFA_jack_enable(MFA_ENABLE);
}

void __attribute__ ((signal)) algo_00(void)
{
	MFA_input_t in;
	MFA_output_t out = { 0 };

	in.acc[0] = (float)cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	in.acc[1] = (float)cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	in.acc[2] = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	in.gyr[0] = (float)cast_sint16_t(ISPU_GRAW_X) * GYR_SENS;
	in.gyr[1] = (float)cast_sint16_t(ISPU_GRAW_Y) * GYR_SENS;
	in.gyr[2] = (float)cast_sint16_t(ISPU_GRAW_Z) * GYR_SENS;
	uint32_t prs_raw = (uint32_t)cast_uint16_t(ISPU_ERAW_0) + ((uint32_t)cast_uint8_t(ISPU_ERAW_1) << 16);
	in.prs = (float)prs_raw * PRS_SENS;
	in.dtime = dtime;

	MotionFA_update(&out, &in);

	cast_float(ISPU_DOUT_00) = in.acc[0];
	cast_float(ISPU_DOUT_02) = in.acc[1];
	cast_float(ISPU_DOUT_04) = in.acc[2];
	cast_float(ISPU_DOUT_06) = in.gyr[0];
	cast_float(ISPU_DOUT_08) = in.gyr[1];
	cast_float(ISPU_DOUT_10) = in.gyr[2];
	cast_float(ISPU_DOUT_12) = in.prs;
	cast_uint16_t(ISPU_DOUT_14) = out.bicep;
	cast_uint16_t(ISPU_DOUT_15) = out.squat;
	cast_uint16_t(ISPU_DOUT_16) = out.push_up;
	cast_uint16_t(ISPU_DOUT_17) = out.lateral;
	cast_uint16_t(ISPU_DOUT_18) = out.overhead;
	cast_uint16_t(ISPU_DOUT_19) = out.jack;

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

