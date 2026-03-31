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


.globl ispu_absmax_f32
.section .text.ispu_absmax_f32, "ax", @progbits
.type ispu_absmax_f32, @function
.align 4
ispu_absmax_f32:
	subw %r15, #4                               ; prologue
	stw [%r15]-, %r4                            ; prologue
	stw [%r15]-, %r5                            ; prologue
	stw [%r15], %r6                             ; prologue

	movw %r6, %r3                               ; r6 = blocksize
	subw %r6, #1
	slaw %r6, #2
	addw %r0, %r6                               ; r0 = &src[block_size - 1]

	movw %r5, #0xFF800000                       ; r5 = -Inf

	movw %r4, .ispu_absmax_f32.inner_loop_start ; ISPU_LOOP_START = .ispu_absmax_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r4
	movw %r4, .ispu_absmax_f32.inner_loop_end   ; ISPU_LOOP_END = .ispu_absmax_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r4

.ispu_absmax_f32.loop_start:
	movw %r4, %r3
	cmpw %r3, #0x400
	jpdlt .ispu_absmax_f32.load_loop_cnt
	movw %r4, #0x400                            ; r4 = block_size < 1024 ? block_size : 1024

.ispu_absmax_f32.load_loop_cnt:
	subw %r4, #1                                ; ISPU_LOOP_CNT = 0x400 | (r4 - 1)
	bset %r4, #10
	stw [ISPU_LOOP_CNT], %r4

.ispu_absmax_f32.inner_loop_start:
	ldw %r4, [%r0]                              ; r4 = src[i++]
	bclear %r4, #0x1F                           ; r4 = fabsf(r4)
	fmax %r5, %r4                               ; r5 = r5 > r4 ? r5 : r4
	cmpw %r5, %r4
	jpdne .ispu_absmax_f32.inner_loop_end
	movw %r6, %r0                               ; r6 = r5 != r4 ? r6 : r0
.ispu_absmax_f32.inner_loop_end:
	subw %r0, #4                                ; src--

	subw %r3, #0x400                            ; block_size -= 1024
	cmpw %r3, #0x1
	jpdge .ispu_absmax_f32.loop_start

.ispu_absmax_f32.loop_end:
	stw [%r1], %r5                              ; *res = r5
	addw %r0, #4
	subw %r6, %r0
	sraw %r6, #2
	stw [%r2], %r6                              ; *idx = (addr_max - src) >> 2

	ldw %r6, [%r15]+                            ; epilogue
	ldw %r5, [%r15]+                            ; epilogue
	ldw %r4, [%r15]+                            ; epilogue
	ret
.size ispu_absmax_f32, .-ispu_absmax_f32

