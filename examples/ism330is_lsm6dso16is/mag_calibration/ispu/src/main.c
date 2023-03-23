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
#include "motion_mc.h"

#define MAG_SENS 0.003f

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

static float dtime;
static void *mmc;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	(void)memcpy((void *)&dtime, (void *)&dt, sizeof(float));
	dtime *= 2.0f;

	if (mmc != NULL) {
		MotionMC_deinitialize(mmc);
	}
	mmc = MotionMC_initialize(MMC_HI_ONLY);
}

void __attribute__ ((signal)) algo_00(void)
{
	MMC_input_t mc_in;
	MMC_output_t mc_out;
	(void)memset(&mc_out, 0, sizeof(mc_out));

	mc_in.mag[0] = (float)cast_sint16_t(ISPU_ERAW_0) * MAG_SENS;
	mc_in.mag[1] = (float)cast_sint16_t(ISPU_ERAW_1) * MAG_SENS;
	mc_in.mag[2] = (float)cast_sint16_t(ISPU_ERAW_2) * MAG_SENS;
	mc_in.dtime = dtime;
	MotionMC_update(mmc, &mc_out, &mc_in);

	cast_float(ISPU_DOUT_00) = mc_in.mag[0] - mc_out.HI_bias[0];
	cast_float(ISPU_DOUT_02) = mc_in.mag[1] - mc_out.HI_bias[1];
	cast_float(ISPU_DOUT_04) = mc_in.mag[2] - mc_out.HI_bias[2];
	cast_float(ISPU_DOUT_06) = mc_out.HI_bias[0];
	cast_float(ISPU_DOUT_08) = mc_out.HI_bias[1];
	cast_float(ISPU_DOUT_10) = mc_out.HI_bias[2];
	cast_uint8_t(ISPU_DOUT_12) = (uint8_t)mc_out.cal_quality;

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

