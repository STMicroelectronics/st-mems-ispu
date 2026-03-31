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
#include "ispu_dsp.h"

#include <stdint.h>
#include <stdbool.h>

#define ACC_SENS 0.000095f
#define BLOCK_SIZE 25
#define FIR_TAPS 25
#define FIR_STATE_SIZE (FIR_TAPS + BLOCK_SIZE - 1)

static volatile uint16_t int_status;

static volatile uint16_t cnt, out_cnt;
static volatile uint8_t full, fill, proc;
static volatile float acc_x[2][BLOCK_SIZE], acc_y[2][BLOCK_SIZE], acc_z[2][BLOCK_SIZE];
static float out_x[2][BLOCK_SIZE], out_y[2][BLOCK_SIZE], out_z[2][BLOCK_SIZE];

static struct ispu_fir_instance_f32 fir_x, fir_y, fir_z;
static const float coeffs[FIR_TAPS] = { [0 ... FIR_TAPS - 1] = 1.0f / FIR_TAPS };
static float state_x[FIR_STATE_SIZE], state_y[FIR_STATE_SIZE], state_z[FIR_STATE_SIZE];

void __attribute__ ((signal)) data_valid_interrupt(void)
{
	acc_x[fill][cnt] = sint20_to_float(cast_sint32_t(ISPU_ARAW_X)) * ACC_SENS;
	acc_y[fill][cnt] = sint20_to_float(cast_sint32_t(ISPU_ARAW_Y)) * ACC_SENS;
	acc_z[fill][cnt] = sint20_to_float(cast_sint32_t(ISPU_ARAW_Z)) * ACC_SENS;
	cnt++;

	if (cnt == BLOCK_SIZE) {
		cnt = 0;
		full = 1;
		fill = !fill;
		proc = !proc;
	}
}

void __attribute__ ((signal)) algo_00_init(void)
{
	cnt = 0;
	out_cnt = 1;
	full = 0;
	fill = 0;
	proc = 1;

	ispu_fir_init_f32(&fir_x, FIR_TAPS, coeffs, state_x, BLOCK_SIZE);
	ispu_fir_init_f32(&fir_y, FIR_TAPS, coeffs, state_y, BLOCK_SIZE);
	ispu_fir_init_f32(&fir_z, FIR_TAPS, coeffs, state_z, BLOCK_SIZE);
}

void __attribute__ ((signal)) algo_00(void)
{
	if (full) {
		full = 0;

		ispu_fir_f32(&fir_x, (float *)acc_x[proc], out_x[proc], BLOCK_SIZE);
		ispu_fir_f32(&fir_y, (float *)acc_y[proc], out_y[proc], BLOCK_SIZE);
		ispu_fir_f32(&fir_z, (float *)acc_z[proc], out_z[proc], BLOCK_SIZE);
	}

	cast_float(ISPU_DOUT_00) = acc_x[proc][out_cnt];
	cast_float(ISPU_DOUT_02) = acc_y[proc][out_cnt];
	cast_float(ISPU_DOUT_04) = acc_z[proc][out_cnt];

	cast_float(ISPU_DOUT_06) = out_x[proc][out_cnt];
	cast_float(ISPU_DOUT_08) = out_y[proc][out_cnt];
	cast_float(ISPU_DOUT_10) = out_z[proc][out_cnt];

	if (++out_cnt == BLOCK_SIZE)
		out_cnt = 0;

	int_status = int_status | 0x1u;
}

int main(void)
{
	// set boot end flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x40u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable high-priority (data valid) interrupt and algorithms interrupt request generation
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x09;

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

