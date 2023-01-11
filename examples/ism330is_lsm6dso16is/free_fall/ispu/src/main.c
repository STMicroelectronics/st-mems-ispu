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
#include "free_fall.h"

#define ACC_SENS 0.000488f
#define FF_THS 0.3f
#define FF_DUR 0.25f

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

static float dtime;
static struct free_fall ff;

void __attribute__ ((signal)) algo_00_init(void)
{
	uint32_t dt = ((uint32_t)cast_uint16_t(ISPU_DTIME_1) << 16) | cast_uint16_t(ISPU_DTIME_0);
	(void)memcpy((void *)&dtime, (void *)&dt, sizeof(float));

	struct free_fall_conf ff_conf = {
		.ths = FF_THS,
		.dur = FF_DUR
	};

	free_fall_init(&ff, &ff_conf);
}

void __attribute__ ((signal)) algo_00(void)
{
	struct free_fall_in in;

	in.acc[0] = (float)cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	in.acc[1] = (float)cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	in.acc[2] = (float)cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
	in.dtime = dtime;

	uint8_t ff_status = free_fall_run(&ff, &in);

	cast_float(ISPU_DOUT_00) = in.acc[0];
	cast_float(ISPU_DOUT_02) = in.acc[1];
	cast_float(ISPU_DOUT_04) = in.acc[2];
	cast_uint8_t(ISPU_DOUT_06) = ff_status;

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

