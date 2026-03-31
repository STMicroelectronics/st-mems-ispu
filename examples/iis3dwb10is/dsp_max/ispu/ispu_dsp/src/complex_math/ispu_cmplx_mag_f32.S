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

.globl ispu_cmplx_mag_f32
.section .text.ispu_cmplx_mag_f32, "ax", @progbits
.type ispu_cmplx_mag_f32, @function
.align 4
ispu_cmplx_mag_f32:
	subw %r15, #4
	stw [%r15]-, %r4
	stw [%r15]-, %r5
	stw [%r15]-, %r6
	stw [%r15], %r7

	movw %r3, .ispu_cmplx_mag_f32.inner_loop_start ; ISPU_LOOP_START = .ispu_cmplx_mag_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r3
	movw %r3, .ispu_cmplx_mag_f32.inner_loop_end   ; ISPU_LOOP_END = .ispu_cmplx_mag_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r3

	movw %r6, ISPU_SQRT_I
	movw %r7, ISPU_SQRT_O

.ispu_cmplx_mag_f32.loop_start:
	movw %r3, %r2
	cmpw %r2, #0x400
	jpdlt .ispu_cmplx_mag_f32.load_loop_cnt
	movw %r3, #0x400                               ; r3 = block_size < 1024 ? block_size : 1024

.ispu_cmplx_mag_f32.load_loop_cnt:
	cmpw %r3, #1
	subw %r3, #2                                   ; ISPU_LOOP_CNT = 0x400 | (r3 - 2)
	bset %r3, #10
	stw [ISPU_LOOP_CNT], %r3

	ldw %r3, [%r0]+                                ; r3 = src[i++]
	ldw %r4, [%r0]+                                ; r4 = src[i++]
	fmuls %r3, %r3                                 ; r3 = r3 ^ 2
	fmuls %r4, %r4                                 ; r4 = r4 ^ 2
	fadds %r3, %r4                                 ; r3 = r3 + r4
	stw [%r6], %r3                                 ; hw_sqrtf(r3)
	jpdne .ispu_cmplx_mag_f32.inner_loop_start     ; manage block_size = 1 corner case
	nop
	nop
	nop
	ldw %r5, [%r7]                                 ; store the only sqrt result
	stw [%r1], %r5                                 ; go to the end of the loop
	jpda .ispu_cmplx_mag_f32.loop_end

.ispu_cmplx_mag_f32.inner_loop_start:
	ldw %r3, [%r0]+                                ; r3 = src[i++]
	ldw %r4, [%r0]+                                ; r4 = src[i++]
	fmuls %r3, %r3                                 ; r3 = r3 ^ 2
	ldw %r5, [%r7]                                 ; r5 = hw_sqrtf(previous)
	fmuls %r4, %r4                                 ; r4 = r4 ^ 2
	stw [%r1]+, %r5                                ; store hw_sqrtf(previous) result
	fadds %r3, %r4                                 ; r3 = r3 + r4
.ispu_cmplx_mag_f32.inner_loop_end:
	stw [%r6], %r3                                 ; hw_sqrtf(r3)

.ispu_cmplx_mag_f32.last_result:
	subw %r2, #0x400                               ; block_size -= 1024
	cmpw %r2, #0x1
	nop
	ldw %r5, [%r7]                                 ; store the last hw_sqrtf result
	stw [%r1]+, %r5

	jpdge .ispu_cmplx_mag_f32.loop_start

.ispu_cmplx_mag_f32.loop_end:
	ldw %r7, [%r15]+
	ldw %r6, [%r15]+
	ldw %r5, [%r15]+
	ldw %r4, [%r15]+
	ret
.size ispu_cmplx_mag_f32, .-ispu_cmplx_mag_f32

