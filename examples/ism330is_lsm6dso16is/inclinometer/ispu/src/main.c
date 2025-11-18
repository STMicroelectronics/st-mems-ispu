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
#include <stddef.h>
#include <string.h>
#include "motion_ac.h"
#include "motion_fx.h"

#define ACC_SENS 0.000244f
#define GYR_SENS 0.070f

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

static float dtime, dtime_ms;
static uint32_t n;
static MAC_output_t data_out;
static void *mfx_6x;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	(void)memcpy((void *)&dtime, (void *)&dt, sizeof(float));
	dtime *= 2.0f;
	dtime_ms = dtime * 1000.0f;

	MotionAC_Initialize(1);

	MAC_knobs_t knobs;
	MotionAC_GetKnobs(&knobs);
	knobs.Sample_ms = (uint32_t)dtime_ms;
	(void)MotionAC_SetKnobs(&knobs);

	n = 0;
	MotionAC_GetCalParams(&data_out);

	if (mfx_6x != NULL) {
		MotionFX_deinitialize(mfx_6x);
	}
	mfx_6x = MotionFX_initialize(MFX_6X);
	MotionFX_enable_gbias(mfx_6x, MFX_ENGINE_ENABLE);
	MotionFX_enable_euler(mfx_6x, MFX_ENGINE_ENABLE);
}

void __attribute__ ((signal)) algo_00(void)
{
	MAC_input_t data_in;
	uint8_t cal = 0;

	MFX_input_t in;
	MFX_output_t out;
	(void)memset(&out, 0, sizeof(out));

	data_in.Acc[0] = (float)cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	data_in.Acc[1] = (float)cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	data_in.Acc[2] = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	float ts = (float)n * dtime_ms;
	data_in.TimeStamp = (int)ts;
	n++;

	MotionAC_Update(&data_in, &cal);

	if (cal == 1u) {
		MotionAC_GetCalParams(&data_out);
	}

	in.acc[0] = (data_in.Acc[0] - data_out.AccBias[0]) * data_out.SF_Matrix[0][0];
	in.acc[1] = (data_in.Acc[1] - data_out.AccBias[1]) * data_out.SF_Matrix[1][1];
	in.acc[2] = (data_in.Acc[2] - data_out.AccBias[2]) * data_out.SF_Matrix[2][2];
	in.gyro[0] = (float)cast_sint16_t(ISPU_GRAW_X) * GYR_SENS;
	in.gyro[1] = (float)cast_sint16_t(ISPU_GRAW_Y) * GYR_SENS;
	in.gyro[2] = (float)cast_sint16_t(ISPU_GRAW_Z) * GYR_SENS;
	MotionFX_update(mfx_6x, &out, &in, dtime);

	cast_float(ISPU_DOUT_00) = out.rotation[1];
	cast_float(ISPU_DOUT_02) = out.rotation[2];
	cast_uint8_t(ISPU_DOUT_04) = (uint8_t)data_out.CalQuality;

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

