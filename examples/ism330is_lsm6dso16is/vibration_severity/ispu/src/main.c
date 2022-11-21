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
#include "motion_vs.h"

#define ACC_SENS (0.061f * 9.80665f)

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

static float dtime;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	(void)memcpy((void *)&dtime, (void *)&dt, sizeof(float));
	dtime *= 0.03125f;

	MVS_conf_t conf = {
		.axis = MVS_AXIS_Z,
		.bw = MVS_BW_2_1000_HZ,
		.hp_en = 0,
		.period = 1666,
		.rms_update_period = 119
	};

	MotionVS_initialize(&conf);
}

void __attribute__ ((signal)) algo_00(void)
{
	MVS_input_t in;
	MVS_output_t out;
	(void)memset(&out, 0, sizeof(out));

	in.acc[0] = 0.0f;
	in.acc[1] = 0.0f;
	in.acc[2] = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	in.dtime = dtime;

	MotionVS_update(&out, &in);

	cast_float(ISPU_DOUT_00) = in.acc[2];
	cast_float(ISPU_DOUT_02) = out.acc_filt;
	cast_float(ISPU_DOUT_04) = out.vel;
	cast_float(ISPU_DOUT_06) = out.vel_rms;

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

