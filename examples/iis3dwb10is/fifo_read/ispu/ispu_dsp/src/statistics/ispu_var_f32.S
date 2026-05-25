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

.globl ispu_var_f32
.section .text.ispu_var_f32, "ax", @progbits
.type ispu_var_f32, @function
.align 4
ispu_var_f32:
	subw %r15, #4                            ; prologue
	stw [%r15]-, %r4                         ; prologue
	stw [%r15]-, %r5                         ; prologue
	stw [%r15]-, %r6                         ; prologue
	stw [%r15]-, %r7                         ; prologue
	stw [%r15]-, %r8                         ; prologue
	stw [%r15], %r14                         ; prologue

	movw %r4, %r0                            ; r4 = src
	movw %r5, %r1                            ; r5 = res
	movw %r6, %r2                            ; r6 = block_size
	jli ispu_mean_f32
	ldw %r7, [%r5]                           ; r7 = mean
	movw %r0, #0                             ; r0 = 0.0f
	movw %r1, #0                             ; r1 = 0.0f
	movw %r2, %r6                            ; r2 = r6

	movw %r3, .ispu_var_f32.inner_loop_start ; ISPU_LOOP_START = .ispu_var_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r3
	movw %r3, .ispu_var_f32.inner_loop_end   ; ISPU_LOOP_END = .ispu_var_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r3

.ispu_var_f32.loop_start:
	movw %r3, %r2
	cmpw %r2, #0x400
	jpdlt .ispu_var_f32.load_loop_cnt
	movw %r3, #0x400                         ; r3 = block_size < 1024 ? block_size : 1024

.ispu_var_f32.load_loop_cnt:
	subw %r3, #1                             ; ISPU_LOOP_CNT = 0x400 | (r3 - 1)
	bset %r3, #10
	stw [ISPU_LOOP_CNT], %r3

.ispu_var_f32.inner_loop_start:
	ldw %r8, [%r4]+                          ; r8 = src[i++]
	fsubs %r8, %r7                           ; r8 = r8 - mean
.ispu_var_f32.inner_loop_end:
	acms %r8, %r8                            ; r0 += r8 * r8

	fadds %r0, %r1                           ; r0 += r1
	subw %r2, #0x400                         ; block_size -= 1024
	cmpw %r2, #0x1
	jpdge .ispu_var_f32.loop_start

.ispu_var_f32.loop_end:
	subw %r6, #1
	movw %r7, %r0
	movw %r0, %r6
	jli __floatunsisf                        ; r0 = (float)(block_size - 1)
	movw %r1, %r0
	movw %r0, %r7
	jli __divsf3                             ; r0 = sum / (float)(block_size - 1)
	stw [%r5], %r0                           ; *res = r0

	ldw %r14, [%r15]+                        ; epilogue
	ldw %r8, [%r15]+                         ; epilogue
	ldw %r7, [%r15]+                         ; epilogue
	ldw %r6, [%r15]+                         ; epilogue
	ldw %r5, [%r15]+                         ; epilogue
	ldw %r4, [%r15]+                         ; epilogue
	ret
.size ispu_var_f32, .-ispu_var_f32

