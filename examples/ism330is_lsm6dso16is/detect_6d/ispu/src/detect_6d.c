/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include <math.h>
#include "iir2.h"
#include "detect_6d.h"

static inline float deg2rad(float x)
{
	return x * 0.017453292f;
}

void detect_6d_init(struct detect_6d *d6d, const struct detect_6d_conf *conf)
{
	d6d->conf = *conf;

	d6d->status = D6D_NULL;
	d6d->axis_prev = D6D_NULL;
	d6d->ths = sinf(deg2rad(conf->ths));
	d6d->dur = 0.0f;

	for (uint8_t i = 0u; i < 3u; i++) {
		iir2_init(&d6d->filt[i], &d6d->conf.filt);
	}
}

uint8_t detect_6d_run(struct detect_6d *d6d, const struct detect_6d_in *in)
{
	uint8_t axis = D6D_NULL;
	uint8_t n_axes = d6d->conf.d4 ? 2u : 3u;

	for (uint8_t i = 0u; i < n_axes; i++) {
		float acc_filt = iir2_run(&d6d->filt[i], in->acc[i]);
		if (fabsf(acc_filt) > d6d->ths) {
			if (axis != D6D_NULL) {
				axis = D6D_NULL;
				break;
			}
			uint8_t pos = (acc_filt > 0.0f) ? 1u : 0u;
			axis = (2u * i) + 1u + pos;
		}
	}

	if (axis != d6d->axis_prev) {
		d6d->dur = 0.0f;
	}

	if ((axis != D6D_NULL) && (d6d->dur < d6d->conf.dur)) {
		d6d->dur += in->dtime;
	}

	if (d6d->dur >= d6d->conf.dur) {
		d6d->status = axis;
	}

	d6d->axis_prev = axis;

	return d6d->status;
}

