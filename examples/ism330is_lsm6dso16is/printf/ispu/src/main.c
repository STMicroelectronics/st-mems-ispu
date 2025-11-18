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
#include <stdio.h>
#include <string.h>

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static volatile uint32_t int_status;

static uint8_t cnt;

void __attribute__ ((signal)) algo_00_init(void)
{
	cnt = 0u;
}

void __attribute__ ((signal)) algo_00(void)
{
	if (cnt < 52u) {
		(void)printf("This example uses the \"printf\"");
	} else if (cnt < 104u) {
		(void)printf("to write to the output registers.");
	} else if (cnt < 156u) {
		(void)printf("This message loops every 6 seconds.");
	} else {
		// no action
	}

	if (++cnt == 156u) {
		cnt = 0u;
	}

	(void)fflush(stdout);

	int_status = int_status | 0x1u;
}

int _write(int fd, const void *buf, size_t count)
{
	(void)fd; // not used, always write to output registers

	size_t bytes = count;

	if (bytes > 63u) {
		bytes = 63u;
	}

	(void)memcpy((char *)ISPU_DOUT_00, buf, bytes);
	cast_char(ISPU_DOUT_00 + (int32_t)bytes) = '\0';

	return (int)bytes;
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

