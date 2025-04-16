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
#include <stdbool.h>
#include "iir2.h"

void iir2_init(struct iir2 *f, const struct iir2_conf *conf)
{
	f->conf = *conf;

	for (uint8_t i = 0; i < 3u; i++) {
		f->w[i] = 0.0f;
	}

	f->fast_set = f->conf.fast_set;
}

float iir2_run(struct iir2 *f, float x)
{
	float y;

	if (f->fast_set) {
		float w = x / (1.0f + f->conf.a[1] + f->conf.a[2]);

		for (uint8_t i = 0; i < 3u; i++) {
			f->w[i] = w;
		}

		f->fast_set = 0;
	}

	f->w[0] = f->w[1];
	f->w[1] = f->w[2];
	f->w[2] = (x - (f->conf.a[2] * f->w[0])) - (f->conf.a[1] * f->w[1]);
	y = ((f->conf.b[0] * f->w[2]) + (f->conf.b[2] * f->w[0])) + (f->conf.b[1] * f->w[1]);

	return y;
}

