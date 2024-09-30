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

#define NUM_AXES 3
#define ACC_SENS_G 0.000244f
#define F_SAMPLE 52.0f
#define FULL_SCALE_G 8
#define RESET_LEN_SAMPLES 52
#define TRIG_LEN_SAMPLES 5
#define WIN_LEN_SAMPLES 104
#define WIN_THS_G 0.25f
#define NUM_LABELS 4
#define PRED_THS 0.6f

const float IIR2_B[] = { 0.19773946280316276f, 0.0f, -0.19773946280316276 };
const float IIR2_A[] = { 1.0f, -1.5451525924244163f, 0.6045210743936745 };
const char LABELS[] = { 'I', 'S', 'P', 'U', ' ' };

enum {
	LABEL_I,
	LABEL_S,
	LABEL_P,
	LABEL_U,
	LABEL_NULL,
};

#endif //ISPU_WAND_PARAMS_H
