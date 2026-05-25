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

#define FIFO_WTM 250

static void process_fifo(void);

static volatile uint16_t int_status;

static volatile uint8_t fifo_int;

void __attribute__ ((signal)) lo_prio_interrupt(void)
{
	if ((cast_uint8_t(ISPU_LOPRIO_INT) & 0x01) != 0) // check if there is a FIFO interrupt
		fifo_int = 1;
}

void __attribute__ ((signal)) algo_00_init(void)
{
	if (fifo_int) { // process FIFO only if an interrupt was received
		fifo_int = 0;
		process_fifo();
	}
}

void __attribute__ ((signal)) algo_00(void)
{
	if (fifo_int) { // process FIFO only if an interrupt was received
		fifo_int = 0;
		process_fifo();
	}
}

static void process_fifo(void)
{
	uint16_t acc_n = 0, samples = cast_uint16_t(FIFO_CTRL1_ISPU) & 0x0FFF;
	int32_t acc_mean[3] = { 0, 0, 0 };

	if (samples == 0)
		samples = 2048;

	cast_uint8_t(FIFO2ISPU_CTRL) = 0x04; // enable FIFO read

	uint8_t tag = 0;
	do {
		cast_uint8_t(FIFO2ISPU_CTRL) = 0x06; // request entry read
		while ((cast_uint8_t(FIFO2ISPU_CTRL) & 0x01) == 0) // wait for entry read
			;

		tag = cast_uint8_t(FIFO2ISPU_TAG);

		if (tag == 0x10) { // read and process accelerometer data from FIFO entry
			acc_mean[0] += cast_sint32_t(ISPU_ARAW_X);
			acc_mean[1] += cast_sint32_t(ISPU_ARAW_Y);
			acc_mean[2] += cast_sint32_t(ISPU_ARAW_Z);

			acc_n++;
		}

		cast_uint8_t(FIFO2ISPU_CTRL) = 0x04; // clear request
		while ((cast_uint8_t(FIFO2ISPU_CTRL) & 0x01) != 0)
			;
	} while (tag && acc_n < samples); // loop until FIFO is empty or the configured number of samples are read

	cast_uint8_t(FIFO2ISPU_CTRL) = 0x00; // disable FIFO read

	if (acc_n > 0) {
		acc_mean[0] /= acc_n;
		acc_mean[1] /= acc_n;
		acc_mean[2] /= acc_n;
	}

	cast_sint32_t(ISPU_DOUT_00) = acc_mean[0];
	cast_sint32_t(ISPU_DOUT_02) = acc_mean[1];
	cast_sint32_t(ISPU_DOUT_04) = acc_mean[2];
	cast_uint16_t(ISPU_DOUT_06) = acc_n;

	int_status = int_status | 0x1u;
}

int main(void)
{
	// set boot end flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x40u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable low-priority interrupt and algorithms interrupts
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x05;

	// configure FIFO
	cast_uint16_t(FIFO_CTRL1_ISPU) = FIFO_WTM <= 2048 ? FIFO_WTM : 2048; // watermark threshold
	cast_uint8_t(FIFO_CTRL3_ISPU) = 0x0A; // continuous mode + batch accelerometer

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

