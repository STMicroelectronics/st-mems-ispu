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
#include <math.h>

void ispu_hamming_f32(float *dst, uint32_t block_size)
{
	float k = 2.0f * (float)M_PI / (float)block_size;

	for (uint32_t i = 0; i < block_size; i++)
		dst[i] = 0.54f - 0.46f * cosf(i * k);
}

