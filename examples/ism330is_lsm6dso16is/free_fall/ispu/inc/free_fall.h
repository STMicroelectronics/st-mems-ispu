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

#ifndef FREE_FALL_H
#define FREE_FALL_H

#include <stdint.h>

#define FF_NOT_DETECTED 0u
#define FF_DETECTED 1u

struct free_fall_conf {
	float ths; // Free-fall threshold on accelerometer axes [g]
	float dur; // Free-fall duration threshold [s]
};

struct free_fall {
	struct free_fall_conf conf;
	float dur;
};

struct free_fall_in {
	float acc[3]; // Accelerometer data [g]
	float dtime; // Delta-time [s]
};

void free_fall_init(struct free_fall *ff, const struct free_fall_conf *conf);
uint8_t free_fall_run(struct free_fall *ff, const struct free_fall_in *in);

#endif

