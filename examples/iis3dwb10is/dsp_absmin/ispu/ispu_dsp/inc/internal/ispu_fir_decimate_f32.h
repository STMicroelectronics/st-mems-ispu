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

#ifndef ISPU_FIR_DECIMATE_F32_H
#define ISPU_FIR_DECIMATE_F32_H

#include "ispu_dsp.h"

#include <stdint.h>

void ispu_fir_decimate_f32_small(const struct ispu_fir_decimate_instance_f32 *inst, const float *src, float *dst, uint32_t block_size);
void ispu_fir_decimate_f32_big(const struct ispu_fir_decimate_instance_f32 *inst, const float *src, float *dst, uint32_t block_size);

void ispu_fir_decimate_f32_opt_odd(const struct ispu_fir_decimate_instance_f32_opt *inst, const float *src, float *dst, uint32_t block_size);
void ispu_fir_decimate_f32_opt_even(const struct ispu_fir_decimate_instance_f32_opt *inst, const float *src, float *dst, uint32_t block_size);

#endif

