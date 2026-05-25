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

.globl ispu_hilbert_f32
.section .text.ispu_hilbert_f32, "ax", @progbits
.type ispu_hilbert_f32, @function
.align 4
ispu_hilbert_f32:
	subw %r15, #4                                        ; prologue
	stw [%r15]-, %r4                                     ; prologue
	stw [%r15]-, %r5                                     ; prologue
	stw [%r15]-, %r6                                     ; prologue
	stw [%r15]-, %r7                                     ; prologue
	stw [%r15]-, %r8                                     ; prologue
	stw [%r15]-, %r9                                     ; prologue
	stw [%r15], %r14                                     ; prologue

	movw %r4, %r1                                        ; r4 = src
	movw %r5, %r2                                        ; r5 = dst
	movw %r3, .ispu_hilbert_f32.in_phase_loop_start      ; ISPU_LOOP_START = .ispu_hilbert_f32.in_phase_loop_start
	movw %r6, %r0                                        ; r6 = inst->rfft_inst
	addw %r0, #8
	stw [ISPU_LOOP_START], %r3
	movw %r3, .ispu_hilbert_f32.in_phase_loop_end        ; ISPU_LOOP_END = .ispu_hilbert_f32.in_phase_loop_end
	movw %r7, #0
	ldh %r7, [%r0]                                       ; r7 = inst->hilbert_len
	stw [ISPU_LOOP_END], %r3
	sraw %r7, #1                                         ; r7 = inst->hilbert_len / 2
	subw %r7, #1
	bset %r7, #10
	stw [ISPU_LOOP_CNT], %r7
	ldw %r8, [%r1]+                                      ; r8 = src[2 * i]
	ldw %r9, [%r1]+                                      ; r9 = src[2 * i + 1]

.ispu_hilbert_f32.in_phase_loop_start:
	stw [%r2], %r8                                       ; dst[(2 * i) * 2] = r8
	ldw %r8, [%r1]+                                      ; r8 = src[2 * i]
	addw %r2, #8                                         ; r5 = &dst[(2 * i + 1) * 2]
	stw [%r2], %r9                                       ; dst[(2 * i + 1) * 2] = r9
	ldw %r9, [%r1]+                                      ; r9 = src[2 * i + 1]
.ispu_hilbert_f32.in_phase_loop_end:
	addw %r2, #8

	movw %r8, %r2                                        ; r8 = r2
	movw %r1, %r4                                        ; r1 = src
	movw %r0, %r6                                        ; r0 = inst->rfft->inst

	jli ispu_rfft_f32

	movw %r0, %r6
	movw %r1, %r4
	movw %r2, %r8

	movw %r3, .ispu_hilbert_f32.ninty_degree_shift_start ; ISPU_LOOP_START = .ispu_hilbert_f32.in_phase_loop_start
	stw [ISPU_LOOP_START], %r3
	movw %r3, .ispu_hilbert_f32.ninty_degree_shift_end   ; ISPU_LOOP_END = .ispu_hilbert_f32.in_phase_loop_end
	stw [ISPU_LOOP_END], %r3
	bset %r7, #10
	stw [ISPU_LOOP_CNT], %r7
	movw %r1, %r4
	movw %r3, %r4
	addw %r4, #4

.ispu_hilbert_f32.ninty_degree_shift_start:              ; Re(X(i)) = Im(X(i)), Im(X(i)) = -Re(X(i))
                                                         ; this is equivalent to a multiplication with 0 - 1j
	ldw %r8, [%r1]                                       ; r8 = src[2 * i]
	ldw %r9, [%r4]                                       ; r9 = src[2 * i + 1]
	binv %r8, #31                                        ; r8 = -src[2 * i]
	stw [%r4], %r8                                       ; src[2 * i + 1] = src[2 * i]
	addw %r4, #8
	stw [%r1], %r9                                       ; src[2 * i] = -src[2 * i + 1]
.ispu_hilbert_f32.ninty_degree_shift_end:
	addw %r1, #8

	movw %r4, %r3
	movw %r8, 0x0
	stw [%r3]+, %r8
	stw [%r3]-, %r8

	movw %r0, %r6                                        ; r0 = inst->rfft->inst
	movw %r1, %r3                                        ; r1 = src
	jli ispu_irfft_f32

	addw %r5, #4                                         ; r5 = &dst[1]

	movw %r3, .ispu_hilbert_f32.quadrature_loop_start    ; ISPU_LOOP_START = .ispu_hilbert_f32.quadrature_loop_start
	stw [ISPU_LOOP_START], %r3
	movw %r3, .ispu_hilbert_f32.quadrature_loop_end      ; ISPU_LOOP_END = .ispu_hilbert_f32.quadrature_loop_end
	stw [ISPU_LOOP_END], %r3
	bset %r7, #10
	stw [ISPU_LOOP_CNT], %r7
	ldw %r8, [%r4]+                                      ; r8 = src[2 * i]
	ldw %r9, [%r4]+                                      ; r9 = src[2 * i + 1]

.ispu_hilbert_f32.quadrature_loop_start:
	stw [%r5], %r8                                       ; dst[(2 * i) * 2 + 1] = r8
	ldw %r8, [%r4]+                                      ; r8 = src[2 * i]
	addw %r5, #8                                         ; r5 = &dst[(2 * i + 1) * 2 + 1]
	stw [%r5], %r9                                       ; dst[(2 * i + 1) * 2 + 1] = r9
	ldw %r9, [%r4]+                                      ; r9 = src[2 * i + 1]
.ispu_hilbert_f32.quadrature_loop_end:
	addw %r5, #8

	ldw %r14, [%r15]+                                    ; epilogue
	ldw %r9, [%r15]+                                     ; epilogue
	ldw %r8, [%r15]+                                     ; epilogue
	ldw %r7, [%r15]+                                     ; epilogue
	ldw %r6, [%r15]+                                     ; epilogue
	ldw %r5, [%r15]+                                     ; epilogue
	ldw %r4, [%r15]+                                     ; epilogue
	ret
.size ispu_hilbert_f32, .-ispu_hilbert_f32

