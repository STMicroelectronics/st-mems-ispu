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

.globl ispu_rms_f32
.section .text.ispu_rms_f32, "ax", @progbits
.type ispu_rms_f32, @function
.align 4
ispu_rms_f32:
	subw %r15, #4          ; prologue
	stw [%r15]-, %r4       ; prologue
	stw [%r15]-, %r5       ; prologue
	stw [%r15]-, %r6       ; prologue
	stw [%r15], %r14       ; prologue

	movw %r4, %r0          ; r4 = src
	movw %r5, %r1          ; r5 = res
	movw %r6, %r2          ; r6 = block_size

	jli ispu_power_f32

	movw %r0, %r6          ; r0 = (float)block_size
	jli __floatunsisf

	movw %r1, %r0          ; r1 = (float)block_size
	ldw %r0, [%r5]         ; r0 = sum_sq
	jli __divsf3           ; r0 = sum_sq / (float)block_size

	stw [ISPU_SQRT_I], %r0 ; sqrtf
	nop
	nop
	nop
	ldw %r0, [ISPU_SQRT_O]

	stw [%r5], %r0         ; *res = sqrtf(r0)

	ldw %r14, [%r15]+      ; epilogue
	ldw %r6, [%r15]+       ; epilogue
	ldw %r5, [%r15]+       ; epilogue
	ldw %r4, [%r15]+       ; epilogue
	ret
.size ispu_rms_f32, .-ispu_rms_f32

