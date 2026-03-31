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

.globl ispu_fir_f32_opt_odd
.section .text.ispu_fir_f32_opt_odd, "ax", @progbits
.type ispu_fir_f32_opt_odd, @function
.align 4
ispu_fir_f32_opt_odd:
	subw %r15, 4
	stw [%r15]-, %r4                                 ; prologue
	stw [%r15]-, %r5                                 ; prologue
	stw [%r15]-, %r6                                 ; prologue
	stw [%r15]-, %r7                                 ; prologue
	stw [%r15]-, %r8                                 ; prologue
	stw [%r15]-, %r9                                 ; prologue
	stw [%r15]-, %r10                                ; prologue
	stw [%r15]-, %r11                                ; prologue
	stw [%r15]-, %r12                                ; prologue
	stw [%r15]-, %r14                                ; prologue

	ldw %r4, [ISPU_CIRCULAR_START]                   ; save ISPU_CIRCULAR_START to the stack
	stw [%r15]-, %r4
	ldw %r4, [ISPU_CIRCULAR_END]                     ; save ISPU_CIRCULAR_END to the stack
	stw [%r15]-, %r4
	ldw %r4, [ISPU_CIRCULAR_CURR]                    ; save ISPU_CIRCULAR_CURR to the stack
	stw [%r15]-, %r4
	ldw %r4, [ISPU_CIRCULAR_STRIDE]                  ; save ISPU_CIRCULAR_STRIDE to the stack
	stw [%r15]-, %r4
	stw [%r15], %r0                                  ; save inst to the stack

	ldh %r5, [%r0]                                   ; r5 = inst->num_taps
	addw %r0, #4
	ldw %r4, [%r0]+                                  ; r4 = inst->state
	stw [ISPU_CIRCULAR_START], %r4                   ; set ISPU_CIRCULAR_START to inst->state
	movw %r6, %r5                                    ; r6 = inst->num_taps
	slaw %r6, #2                                     ; r6 *= 4
	addw %r6, %r4                                    ; r6 += r4
	stw [ISPU_CIRCULAR_END], %r6                     ; set ISPU_CIRCULAR_END to inst->state
	ldw %r6, [%r0]+
	stw [ISPU_CIRCULAR_CURR], %r6                    ; set ISPU_CIRCULAR_CURR to inst->state_curr
	movw %r6, #0
	stw [ISPU_CIRCULAR_STRIDE], %r6                  ; set ISPU_CIRCULAR_STRIDE to 0

	ldw %r6, [%r0]                                   ; r6 = inst->coeffs
	sraw %r5, #1                                     ; r5 /= 2
	subw %r5, #1                                     ; r5 -= 1 ((inst->num_taps / 2) - 1)
	movw %r4, %r1                                    ; r4 = src

	movw %r7, .ispu_fir_f32_opt_odd.inner_loop_start
	stw [ISPU_LOOP_START], %r7
	movw %r7, .ispu_fir_f32_opt_odd.inner_loop_end
	stw [ISPU_LOOP_END], %r7

	movw %r10, ISPU_CIRCULAR_MATCH
	movw %r11, ISPU_LOOP_CNT

.ispu_fir_f32_opt_odd.loop_start:
	ldw %r7, [%r4]+                                  ; %r7 = src[i++]
	movw %r8, %r6                                    ; r8 = inst->coeffs
	stw [%r10], %r7                                  ; store to ISPU_CIRCULAR_MATCH %r7

	bset %r5, #10
	stw [%r11], %r5
	movw %r1, 0x0
	movw %r0, 0x0

.ispu_fir_f32_opt_odd.inner_loop_start:              ; accumulate products between filter and circular buffer in r0
	ldw %r12, [%r10]                                 ; mac operations are performed with unrolling 2 to optimize cpu pipeline
	ldw %r14, [%r8]+
	ldw %r7, [%r10]
	ldw %r9, [%r8]+
	acms %r12, %r14
.ispu_fir_f32_opt_odd.inner_loop_end:
	acms %r7, %r9

	ldw %r7, [%r10]
	ldw %r9, [%r8]+
	subw %r3, #1                                     ; block_size -= 1
	acms %r7, %r9                                    ; perform last mac since num_taps is odd

	fadds %r0, %r1                                   ; add last results
	cmpw %r3, #0
	stw [%r2]+, %r0                                  ; dst[i++] = r0
	jpdne .ispu_fir_f32_opt_odd.loop_start           ; loop for all the elements in the block
.ispu_fir_f32_opt_odd.loop_end:

	ldw %r0, [%r15]+                                 ; load inst	from the stack
	addw %r0, #8                                     ; r0 = inst->state_curr
	ldw %r1, [ISPU_CIRCULAR_CURR]                    ; load ISPU_CIRCULAR_CURR into %r1
	stw [%r0], %r1                                   ; inst->state_curr = ISPU_CIRCULAR_CURR

	ldw %r4, [%r15]+
	stw [ISPU_CIRCULAR_STRIDE], %r4                  ; reload original ISPU_CIRCULAR_STRIDE
	ldw %r4, [%r15]+
	stw [ISPU_CIRCULAR_CURR], %r4                    ; reload original ISPU_CIRCULAR_CURR
	ldw %r4, [%r15]+
	stw [ISPU_CIRCULAR_END], %r4                     ; reload original ISPU_CIRCULAR_END
	ldw %r4, [%r15]+
	stw [ISPU_CIRCULAR_START], %r4                   ; reload original ISPU_CIRCULAR_START

	ldw %r14, [%r15]+                                ; epilogue
	ldw %r12, [%r15]+                                ; epilogue
	ldw %r11, [%r15]+                                ; epilogue
	ldw %r10, [%r15]+                                ; epilogue
	ldw %r9, [%r15]+                                 ; epilogue
	ldw %r8, [%r15]+                                 ; epilogue
	ldw %r7, [%r15]+                                 ; epilogue
	ldw %r6, [%r15]+                                 ; epilogue
	ldw %r5, [%r15]+                                 ; epilogue
	ldw %r4, [%r15]+                                 ; epilogue
	ret
.size ispu_fir_f32_opt_odd, .-ispu_fir_f32_opt_odd

