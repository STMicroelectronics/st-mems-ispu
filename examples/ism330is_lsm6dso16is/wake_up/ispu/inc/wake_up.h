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

#ifndef WAKE_UP_H
#define WAKE_UP_H

#include <stdint.h>
#include "iir2.h"

#define WU_INACTIVE 0u
#define WU_ACTIVE 1u

struct wake_up_conf {
	float ths; // Wake-up and sleep threshold on filtered accelerometer axes [g]
	float wake_dur; // Wake-up duration threshold [s]
	float sleep_dur; // Sleep duration threshold [s]
	struct iir2_conf filt; // IIR2 filter configuration
};

struct wake_up {
	struct wake_up_conf conf;
	uint8_t status;
	float wake_dur;
	float sleep_dur;
	struct iir2 filt[3];
};

struct wake_up_in {
	float acc[3]; // Accelerometer data [g]
	float dtime; // Delta-time [s]
};

void wake_up_init(struct wake_up *wu, const struct wake_up_conf *conf);
uint8_t wake_up_run(struct wake_up *wu, const struct wake_up_in *in);

#endif

