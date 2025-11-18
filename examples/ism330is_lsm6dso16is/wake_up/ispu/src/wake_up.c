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
#include "wake_up.h"

void wake_up_init(struct wake_up *wu, const struct wake_up_conf *conf)
{
	wu->conf = *conf;

	wu->status = WU_INACTIVE;
	wu->wake_dur = 0.0f;
	wu->sleep_dur = 0.0f;

	for (uint8_t i = 0u; i < 3u; i++) {
		iir2_init(&wu->filt[i], &wu->conf.filt);
	}
}

uint8_t wake_up_run(struct wake_up *wu, const struct wake_up_in *in)
{
	bool above_ths = false;

	for (uint8_t i = 0u; i < 3u; i++) {
		float acc_filt = iir2_run(&wu->filt[i], in->acc[i]);
		if (fabsf(acc_filt) > wu->conf.ths) {
			above_ths = true;
		}
	}

	if (above_ths) {
		if (wu->wake_dur < wu->conf.wake_dur) {
			wu->wake_dur += in->dtime;
		}

		wu->sleep_dur = 0.0f;

		if (wu->wake_dur >= wu->conf.wake_dur) {
			wu->status = WU_ACTIVE;
		}
	} else {
		if (wu->sleep_dur < wu->conf.sleep_dur) {
			wu->sleep_dur += in->dtime;
		}

		wu->wake_dur = 0.0f;

		if (wu->sleep_dur >= wu->conf.sleep_dur) {
			wu->status = WU_INACTIVE;
		}
	}

	return wu->status;
}

