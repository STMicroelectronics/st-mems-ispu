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

#ifndef DETECT_6D_H
#define DETECT_6D_H

#include <stdint.h>
#include <stdbool.h>
#include "iir2.h"

#define D6D_NULL 0u
#define D6D_XL 1u
#define D6D_XH 2u
#define D6D_YL 3u
#define D6D_YH 4u
#define D6D_ZL 5u
#define D6D_ZH 6u

struct detect_6d_conf {
	bool d4; // Degenerate to 4D only (x and y)
	float ths; // 6D detect threshold [deg]
	float dur; // 6D detect duration threshold [s]
	struct iir2_conf filt; // IIR2 filter configuration
};

struct detect_6d {
	struct detect_6d_conf conf;
	uint8_t status;
	uint8_t axis_prev;
	float ths;
	float dur;
	struct iir2 filt[3];
};

struct detect_6d_in {
	float acc[3]; // Accelerometer data [g]
	float dtime; // Delta-time [s]
};

void detect_6d_init(struct detect_6d *d6d, const struct detect_6d_conf *conf);
uint8_t detect_6d_run(struct detect_6d *d6d, const struct detect_6d_in *in);

#endif

