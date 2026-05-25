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

.globl ispu_std_f32
.section .text.ispu_std_f32, "ax", @progbits
.type ispu_std_f32, @function
.align 4
ispu_std_f32:
	subw %r15, #4          ; prologue
	stw [%r15]-, %r4       ; prologue
	stw [%r15], %r14       ; prologue

	movw %r4, %r1          ; r4 = res
	jli ispu_var_f32

	ldw %r0, [%r4]
	stw [ISPU_SQRT_I], %r0 ; sqrtf
	nop
	nop
	nop
	ldw %r0, [ISPU_SQRT_O]

	stw [%r4], %r0         ; *res = sqrtf(r0)

	ldw %r14, [%r15]+      ; epilogue
	ldw %r4, [%r15]+       ; epilogue
	ret
.size ispu_std_f32, .-ispu_std_f32

