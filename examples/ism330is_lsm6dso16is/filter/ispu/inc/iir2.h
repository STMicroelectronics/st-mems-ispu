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

#ifndef IIR2_H
#define IIR2_H

struct iir2 {
	float b[3];
	float a[3];
	float w[3];
};

void iir2_init(struct iir2 *f, const float b[3], const float a[3]);
float iir2_run(struct iir2 *f, float x);

#endif

