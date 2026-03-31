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
#define BLOCK_SIZE 2048
#define ODR 40000.0f
#define MAG_NORM (2.0f / (float)BLOCK_SIZE)
#define FREQ_BIN (ODR / (float)BLOCK_SIZE)

static volatile uint16_t int_status;

static volatile uint16_t cnt;
static volatile uint8_t full, fill, proc;
static volatile float __attribute__ ((aligned(8192))) acc_x[2][BLOCK_SIZE];
static volatile float __attribute__ ((aligned(8192))) acc_y[2][BLOCK_SIZE];
static volatile float __attribute__ ((aligned(8192))) acc_z[2][BLOCK_SIZE];

static struct ispu_rfft_instance_f32 rfft_instance;

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
	full = 0;
	fill = 0;
	proc = 1;

	ispu_rfft_init_f32(&rfft_instance, BLOCK_SIZE);
}

void __attribute__ ((signal)) algo_00(void)
{
	float mean_x = 0.0f, mean_y = 0.0f, mean_z = 0.0f;
	float bin_nyq_x = 0.0f, bin_nyq_y = 0.0f, bin_nyq_z = 0.0f;
	float mag_x = 0.0f, mag_y = 0.0f, mag_z = 0.0f, freq_x = 0.0f, freq_y = 0.0f, freq_z = 0.0f;
	uint32_t idx_x = 0, idx_y = 0, idx_z = 0;

	if (full) {
		full = 0;

		ispu_mean_f32((float *)&acc_x[proc][0], &mean_x, BLOCK_SIZE);
		ispu_mean_f32((float *)&acc_y[proc][0], &mean_y, BLOCK_SIZE);
		ispu_mean_f32((float *)&acc_z[proc][0], &mean_z, BLOCK_SIZE);

		ispu_offset_f32((float *)&acc_x[proc][0], -mean_x, (float *)&acc_x[proc][0], BLOCK_SIZE);
		ispu_offset_f32((float *)&acc_y[proc][0], -mean_y, (float *)&acc_y[proc][0], BLOCK_SIZE);
		ispu_offset_f32((float *)&acc_z[proc][0], -mean_z, (float *)&acc_z[proc][0], BLOCK_SIZE);

		ispu_rfft_f32(&rfft_instance, (float *)&acc_x[proc][0]);
		ispu_rfft_f32(&rfft_instance, (float *)&acc_y[proc][0]);
		ispu_rfft_f32(&rfft_instance, (float *)&acc_z[proc][0]);

		bin_nyq_x = acc_x[proc][1];
		bin_nyq_y = acc_y[proc][1];
		bin_nyq_z = acc_z[proc][1];

		ispu_cmplx_mag_f32((float *)&acc_x[proc][2], (float *)&acc_x[proc][1], BLOCK_SIZE / 2 - 1);
		ispu_cmplx_mag_f32((float *)&acc_y[proc][2], (float *)&acc_y[proc][1], BLOCK_SIZE / 2 - 1);
		ispu_cmplx_mag_f32((float *)&acc_z[proc][2], (float *)&acc_z[proc][1], BLOCK_SIZE / 2 - 1);

		acc_x[proc][0] *= 0.5f;
		acc_y[proc][0] *= 0.5f;
		acc_z[proc][0] *= 0.5f;
		acc_x[proc][BLOCK_SIZE / 2] = bin_nyq_x * 0.5f;
		acc_y[proc][BLOCK_SIZE / 2] = bin_nyq_y * 0.5f;
		acc_z[proc][BLOCK_SIZE / 2] = bin_nyq_z * 0.5f;

		ispu_max_f32((float *)&acc_x[proc][0], &mag_x, &idx_x, BLOCK_SIZE / 2 + 1);
		ispu_max_f32((float *)&acc_y[proc][0], &mag_y, &idx_y, BLOCK_SIZE / 2 + 1);
		ispu_max_f32((float *)&acc_z[proc][0], &mag_z, &idx_z, BLOCK_SIZE / 2 + 1);

		mag_x *= MAG_NORM;
		mag_y *= MAG_NORM;
		mag_z *= MAG_NORM;
		freq_x = (float)idx_x * FREQ_BIN;
		freq_y = (float)idx_y * FREQ_BIN;
		freq_z = (float)idx_z * FREQ_BIN;

		cast_float(ISPU_DOUT_00) = mag_x;
		cast_float(ISPU_DOUT_02) = mag_y;
		cast_float(ISPU_DOUT_04) = mag_z;
		cast_float(ISPU_DOUT_06) = freq_x;
		cast_float(ISPU_DOUT_08) = freq_y;
		cast_float(ISPU_DOUT_10) = freq_z;

		int_status = int_status | 0x1u;
	}
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

