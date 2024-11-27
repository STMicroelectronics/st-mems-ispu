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

#ifndef ISPU_WAND_PARAMS_H
#define ISPU_WAND_PARAMS_H

#define F_SAMPLE 52.0f
#define FULL_SCALE_G 8
#define WIN_THS_G 0.25f
#define TRIG_LEN_SAMPLES 5
#define WIN_LEN_SAMPLES 104
#define RESET_LEN_SAMPLES 52
#define ACC_SENS_G 0.000244f
#define PRED_THS 0.6f
#define NUM_AXES 3
#define NUM_LABELS 4
#define IIR2_B { 0.197739f, 0.000000f, -0.197739f }
#define IIR2_A { 1.000000f, -1.545153f, 0.604521f }

enum {
	LABEL_I,
	LABEL_S,
	LABEL_P,
	LABEL_U,
	LABEL_NULL,
};
const char LABELS[] = { 'I', 'S', 'P', 'U', ' ' };

#endif //ISPU_WAND_PARAMS_H
