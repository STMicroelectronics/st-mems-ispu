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

#include <stdint.h>
#include <string.h>

void ispu_biquad_cascade_df1_init_f32(struct ispu_biquad_casd_df1_inst_f32 *inst, uint8_t num_stages, const float *coeffs, float *state)
{
	inst->num_stages = num_stages;
	inst->coeffs = coeffs;
	inst->state = state;

	memset(inst->state, 0, (4 * (uint32_t)num_stages) * sizeof(float));
}

