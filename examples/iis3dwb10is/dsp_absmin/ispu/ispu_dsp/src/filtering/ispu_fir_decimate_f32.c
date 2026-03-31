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

#include "ispu_dsp.h"
#include "internal/ispu_fir_decimate_f32.h"

#include <stdint.h>
#include <string.h>

int ispu_fir_decimate_init_f32(struct ispu_fir_decimate_instance_f32 *inst, uint16_t num_taps, uint8_t dec, const float *coeffs, float *state, uint32_t block_size)
{
	if (block_size % dec != 0)
		return -1;

	inst->num_taps = num_taps;
	inst->dec = dec;
	inst->coeffs = coeffs;
	inst->state = state;
	inst->impl = inst->num_taps <= 1024 ? ispu_fir_decimate_f32_small : ispu_fir_decimate_f32_big;

	memset(inst->state, 0, (num_taps + (block_size - 1)) * sizeof(float));

	return 0;
}

void ispu_fir_decimate_f32(const struct ispu_fir_decimate_instance_f32 *inst, const float *src, float *dst, uint32_t block_size)
{
	inst->impl(inst, src, dst, block_size);
}

int ispu_fir_decimate_init_f32_opt(struct ispu_fir_decimate_instance_f32_opt *inst, uint16_t num_taps, uint8_t dec, const float *coeffs, float *state, uint32_t block_size)
{
	if (block_size % dec != 0 || num_taps > 1024)
		return -1;

	inst->num_taps = num_taps;
	inst->dec = dec;
	inst->coeffs = coeffs;
	inst->state = state;
	inst->state_curr = &state[num_taps - 1];
	inst->impl = inst->num_taps % 2 ? ispu_fir_decimate_f32_opt_odd : ispu_fir_decimate_f32_opt_even;

	memset(inst->state, 0, num_taps * sizeof(float));

	return 0;
}

void ispu_fir_decimate_f32_opt(const struct ispu_fir_decimate_instance_f32_opt *inst, const float *src, float *dst, uint32_t block_size)
{
	inst->impl(inst, src, dst, block_size);
}

