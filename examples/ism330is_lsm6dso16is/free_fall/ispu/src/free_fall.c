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
#include "free_fall.h"

void free_fall_init(struct free_fall *ff, const struct free_fall_conf *conf)
{
	ff->dur = 0.0f;
	ff->conf = *conf;
}

uint8_t free_fall_run(struct free_fall *ff, const struct free_fall_in *in)
{
	uint8_t res = FF_NOT_DETECTED;
	bool below_ths = true;

	for (uint8_t i = 0u; i < 3u; i++) {
		if (fabsf(in->acc[i]) > ff->conf.ths) {
			below_ths = false;
			break;
		}
	}

	if (below_ths) {
		if (ff->dur < ff->conf.dur) {
			ff->dur += in->dtime;
		}
	} else {
		ff->dur = 0.0f;
	}

	if (ff->dur >= ff->conf.dur) {
		res = FF_DETECTED;
	}

	return res;
}

