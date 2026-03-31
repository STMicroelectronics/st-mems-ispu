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

.globl ispu_scale_f32
.section .text.ispu_scale_f32, "ax", @progbits
.type ispu_scale_f32, @function
.align 4
ispu_scale_f32:
	subw %r15, #4                              ; prologue
	stw [%r15], %r4                            ; prologue

	movw %r4, .ispu_scale_f32.inner_loop_start ; ISPU_LOOP_START = .ispu_scale_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r4
	movw %r4, .ispu_scale_f32.inner_loop_end   ; ISPU_LOOP_END = .ispu_scale_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r4

.ispu_scale_f32.loop_start:
	movw %r4, %r3
	cmpw %r3, #0x400
	jpdlt .ispu_scale_f32.load_loop_cnt
	movw %r4, #0x400                           ; r4 = block_size < 1024 ? block_size : 1024

.ispu_scale_f32.load_loop_cnt:
	subw %r4, #1                               ; ISPU_LOOP_CNT = 0x400 | (r4 - 1)
	bset %r4, #10
	stw [ISPU_LOOP_CNT], %r4

.ispu_scale_f32.inner_loop_start:
	ldw %r4, [%r0]+                            ; r4 = src[i++]
	fmuls %r4, %r1                             ; r4 *= r1
.ispu_scale_f32.inner_loop_end:
	stw [%r2]+, %r4                            ; dst[i++] = r4

	subw %r3, #0x400                           ; block_size -= 1024
	cmpw %r3, #0x1
	jpdge .ispu_scale_f32.loop_start

.ispu_scale_f32.loop_end:
	ldw %r4, [%r15]+                           ; epilogue
	ret
.size ispu_scale_f32, .-ispu_scale_f32

