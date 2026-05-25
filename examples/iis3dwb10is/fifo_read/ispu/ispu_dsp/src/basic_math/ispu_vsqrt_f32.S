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

.globl ispu_vsqrt_f32
.section .text.ispu_vsqrt_f32, "ax", @progbits
.type ispu_vsqrt_f32, @function
.align 4
ispu_vsqrt_f32:
	subw %r15, #4                              ; prologue
	stw [%r15]-, %r4                           ; prologue
	stw [%r15]-, %r5                           ; prologue
	stw [%r15]-, %r6                           ; prologue
	stw [%r15]-, %r7                           ; prologue
	stw [%r15], %r8                            ; prologue

	movw %r3, .ispu_vsqrt_f32.inner_loop_start ; ISPU_LOOP_START = .ispu_vsqrt_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r3
	movw %r3, .ispu_vsqrt_f32.inner_loop_end   ; ISPU_LOOP_END = .ispu_vsqrt_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r3

	movw %r4, ISPU_SQRT_I
	movw %r5, ISPU_SQRT_O

.ispu_vsqrt_f32.loop_start:
	movw %r3, %r2
	cmpw %r2, #0x800
	jpdlt .ispu_vsqrt_f32.load_loop_cnt
	movw %r3, #0x800                           ; r3 = block_size < 2048 ? block_size : 2048

.ispu_vsqrt_f32.load_loop_cnt:
	movw %r8, %r3
	bext %r8, #0
	cmpw %r8, #1
	jpdne .ispu_vsqrt_f32.inner_loop_preload   ; if r8 % 2 != 0 precompute one element

	ldw %r6, [%r0]+                            ; r6 = *src++
	stw [%r4], %r6                             ; hw_sqrtf(r6)
	nop
	nop
	nop
	ldw %r7, [%r5]
	stw [%r1]+, %r7                            ; *dst++ = hw_sqrtf(r6)
	subw %r3, #1
	cmpw %r3, #0                               ; r3 == 1 edge case
	jpdeq .ispu_vsqrt_f32.loop_end

.ispu_vsqrt_f32.inner_loop_preload:
	ldw %r6, [%r0]+                            ; r6 = *src++
	ldw %r8, [%r0]+                            ; r8 = *src++
	stw [%r4], %r6                             ; hw_sqrtf(r6)
	sraw %r3, #1                               ; block_size /= 2
	subw %r3, #1                               ; ISPU_LOOP_CNT = 0x400 | (r3 - 1)
	bset %r3, #10
	ldw %r7, [%r5]                             ; r7 = hw_sqrtf(r6)
	stw [ISPU_LOOP_CNT], %r3

.ispu_vsqrt_f32.inner_loop_start:
	stw [%r4], %r8                             ; hw_sqrtf(r8)
	ldw %r6, [%r0]+                            ; r6 = src[i++]
	stw [%r1]+, %r7                            ; dst[i++] = hw_sqrtf(previous)
	ldw %r8, [%r0]+                            ; r8 = src[i++]
	ldw %r7, [%r5]                             ; r7 = hw_sqrtf(previous)
	stw [%r4], %r6                             ; hw_sqrtf(r6)
	stw [%r1]+, %r7                            ; dst[i++] = hw_sqrtf(r8)
	nop
.ispu_vsqrt_f32.inner_loop_end:
	ldw %r7, [%r5]                             ; r7 = hw_sqrtf(r8)

	subw %r2, #0x800                           ; block_size -= 2048
	cmpw %r2, #0x1
	subw %r0, #8
	jpdge .ispu_vsqrt_f32.loop_start

.ispu_vsqrt_f32.loop_end:
	ldw %r8, [%r15]+                           ; epilogue
	ldw %r7, [%r15]+                           ; epilogue
	ldw %r6, [%r15]+                           ; epilogue
	ldw %r5, [%r15]+                           ; epilogue
	ldw %r4, [%r15]+                           ; epilogue
	ret
.size ispu_vsqrt_f32, .-ispu_vsqrt_f32

