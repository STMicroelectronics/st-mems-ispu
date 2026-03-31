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

#include "reg_map.h"

.globl ispu_cmplx_mag_squared_f32
.section .text.ispu_cmplx_mag_squared_f32, "ax", @progbits
.type ispu_cmplx_mag_squared_f32, @function
.align 4
ispu_cmplx_mag_squared_f32:
	movw %r3, .ispu_cmplx_mag_squared_f32.inner_loop_start ; ISPU_LOOP_START = .ispu_cmplx_mag_squared_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r3
	movw %r3, .ispu_cmplx_mag_squared_f32.inner_loop_end   ; ISPU_LOOP_END = .ispu_cmplx_mag_squared_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r3

.ispu_cmplx_mag_squared_f32.loop_start:
	movw %r3, %r2
	cmpw %r2, #0x400
	jpdlt .ispu_cmplx_mag_squared_f32.load_loop_cnt
	movw %r3, #0x400                                       ; r3 = block_size < 1024 ? block_size : 1024

.ispu_cmplx_mag_squared_f32.load_loop_cnt:
	subw %r3, #1                                           ; ISPU_LOOP_CNT = 0x400 | (r3 - 1)
	bset %r3, #10
	stw [ISPU_LOOP_CNT], %r3

.ispu_cmplx_mag_squared_f32.inner_loop_start:
	ldw %r3, [%r0]+                                        ; r3 = src[i++]
	ldw %r4, [%r0]+                                        ; r4 = src[i++]
	fmuls %r3, %r3                                         ; r3 = r3 ^ 2
	fmuls %r4, %r4                                         ; r4 = r4 ^ 2
	fadds %r3, %r4                                         ; r3 = r3 + r4
.ispu_cmplx_mag_squared_f32.inner_loop_end:
	stw [%r1]+, %r3                                        ; dst[i++] = r3

	subw %r2, #0x400                                       ; block_size -= 1024
	cmpw %r2, #0x1
	jpdge .ispu_cmplx_mag_squared_f32.loop_start

.ispu_cmplx_mag_squared_f32.loop_end:
	ret
.size ispu_cmplx_mag_squared_f32, .-ispu_cmplx_mag_squared_f32

