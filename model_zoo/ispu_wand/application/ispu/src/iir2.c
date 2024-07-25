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

#include <stdint.h>
#include "iir2.h"

void iir2_init(struct iir2 *f, const float b[3], const float a[3])
{
	for (uint8_t i = 0; i < 3u; i++) {
		f->b[i] = b[i];
		f->a[i] = a[i];
		f->w[i] = 0.0f;
	}
}

float iir2_run(struct iir2 *f, float x)
{
	float y;

	f->w[0] = f->w[1];
	f->w[1] = f->w[2];
	f->w[2] = (x - (f->a[2] * f->w[0])) - (f->a[1] * f->w[1]);
	y = ((f->b[0] * f->w[2]) + (f->b[2] * f->w[0])) + (f->b[1] * f->w[1]);

	return y;
}

