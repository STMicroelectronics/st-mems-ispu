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
#include "internal/ispu_rfft_f32.h"

#include <stdint.h>

int ispu_rfft_init_f32(struct ispu_rfft_instance_f32 *inst, uint16_t fft_len)
{
	inst->fft_len = fft_len;

	switch (fft_len) {
	case 32:
		inst->twiddle_table = twiddle_table32;
		inst->fft_n_elem_bra = 0;
		break;
	case 64:
		inst->twiddle_table = twiddle_table64;
		inst->fft_n_elem_bra = 1;
		break;
	case 128:
		inst->twiddle_table = twiddle_table128;
		inst->fft_n_elem_bra = 2;
		break;
	case 256:
		inst->twiddle_table = twiddle_table256;
		inst->fft_n_elem_bra = 3;
		break;
	case 512:
		inst->twiddle_table = twiddle_table512;
		inst->fft_n_elem_bra = 4;
		break;
	case 1024:
		inst->twiddle_table = twiddle_table1024;
		inst->fft_n_elem_bra = 5;
		break;
	case 2048:
		inst->twiddle_table = twiddle_table2048;
		inst->fft_n_elem_bra = 6;
		break;
	default:
		return -1;
	}

	return 0;
}

