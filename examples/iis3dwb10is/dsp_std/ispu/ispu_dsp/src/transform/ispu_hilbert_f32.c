/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "ispu_dsp.h"

#include <stdint.h>

int ispu_hilbert_init_f32(struct ispu_hilbert_instance_f32 *inst, uint16_t hilbert_len)
{
	int res = ispu_rfft_init_f32(&inst->rfft_inst, hilbert_len);
	if (res == 0)
		inst->hilbert_len = hilbert_len;

	return res;
}

