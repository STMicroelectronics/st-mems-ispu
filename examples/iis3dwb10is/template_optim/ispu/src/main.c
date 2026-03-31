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

#include <stdint.h>
#include <stdbool.h>

static volatile uint16_t int_status;

void __attribute__ ((signal)) data_valid_interrupt(void)
{
	// TODO: read sensor data
}

void __attribute__ ((signal)) lo_prio_interrupt(void)
{
	// TODO: process the interrupt based on the trigger source(s) indicated by ISPU_LOPRIO_INT
}

static inline void process_data(void)
{
	// TODO: process sensor data

	// interrupt generation (set different bits to distinguish different events)
	int_status = int_status | 0x1u;
}

int main(void)
{
	// set boot end flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x40u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable high-priority (data valid) interrupt, low-priority interrupt, and algorithms interrupts
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x0C;

	while (true) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0u;
		cast_uint16_t(ISPU_INT_STATUS) = 0u;
		cast_uint8_t(ISPU_INT_PIN) = 0u;

		process_data();

		// get interrupt flags
		uint8_t int_pin = 0u;
		int_pin |= ((int_status & cast_uint16_t(ISPU_INT1_CTRL)) > 0u) ? 0x01u : 0x00u;
		int_pin |= ((int_status & cast_uint16_t(ISPU_INT2_CTRL)) > 0u) ? 0x02u : 0x00u;

		// set status registers and generate interrupts
		cast_uint16_t(ISPU_INT_STATUS) = int_status;
		cast_uint8_t(ISPU_INT_PIN) = int_pin;
	}
}

