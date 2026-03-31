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

.globl ispu_fir_f32_small
.section .text.ispu_fir_f32_small, "ax", @progbits
.type ispu_fir_f32_small, @function
.align 4
ispu_fir_f32_small:
	subw %r15, 4
	stw [%r15]-, %r4                               ; prologue
	stw [%r15]-, %r5                               ; prologue
	stw [%r15]-, %r6                               ; prologue
	stw [%r15]-, %r7                               ; prologue
	stw [%r15]-, %r8                               ; prologue
	stw [%r15]-, %r9                               ; prologue
	stw [%r15]-, %r10                              ; prologue
	stw [%r15]-, %r11                              ; prologue
	stw [%r15]-, %r12                              ; prologue
	stw [%r15]-, %r14                              ; prolgoue

	movw %r11, %r1                                 ; r11 = src
	ldh %r9, [%r0]                                 ; r9 = num_taps
	subw %r9, #1                                   ; r9 -= 1
	addw %r0, #4
	ldw %r4, [%r0]+                                ; r4 = state
	subw %r3, #1                                   ; r3 = block_size - 1
	stw [%r15], %r4                                ; save state to stack
	ldw %r5, [%r0]+                                ; r5 = coeffs

	movw %r6, .ispu_fir_f32_small.inner_loop_start
	stw [ISPU_LOOP_START], %r6
	movw %r6, .ispu_fir_f32_small.inner_loop_end
	stw [ISPU_LOOP_END], %r6

	movw %r6, %r9
	slaw %r6, #2
	addw %r6, %r4                                  ; r6 = &state[num_taps - 1]

.ispu_fir_f32_small.loop_start:
	ldw %r12, [%r11]+                              ; r12 = src[i++]
	movw %r7, %r4                                  ; r7 = r4
	stw [%r6]+, %r12                               ; state[i + num_taps - 1] = r12 (load next input sample into state buffer)
	movw %r8, %r5                                  ; r8 = coeffs

	bset %r9, #10
	stw [ISPU_LOOP_CNT], %r9
	movw %r1, 0x0
	movw %r0, 0x0

.ispu_fir_f32_small.inner_loop_start:              ; accumulate products between filter and input mask in r0
	ldw %r10, [%r7]+
	ldw %r14, [%r8]+
.ispu_fir_f32_small.inner_loop_end:
	acms %r10, %r14

	fadds %r0, %r1                                 ; add last results
	stw [%r2]+, %r0                                ; dst[i++] = r0
	addw %r4, #4                                   ; move input mask on
	cmpw %r3, #0
	subw %r3, #1
	jpdne .ispu_fir_f32_small.loop_start           ; loop for all the elements in the block

	ldw %r6, [%r15]+                               ; r6 = state
	movw %r7, .ispu_fir_f32_small.copy_state_start
	stw [ISPU_LOOP_START], %r7
	movw %r7, .ispu_fir_f32_small.copy_state_end
	stw [ISPU_LOOP_END], %r7
	subw %r9, #1
	bset %r9, #10
	stw [ISPU_LOOP_CNT], %r9


.ispu_fir_f32_small.copy_state_start:              ; copy the last num_taps - 1 input samples at state
	ldw %r11, [%r4]+
.ispu_fir_f32_small.copy_state_end:
	stw [%r6]+, %r11

	ldw %r14, [%r15]+                              ; epilogue
	ldw %r12, [%r15]+                              ; epilogue
	ldw %r11, [%r15]+                              ; epilogue
	ldw %r10, [%r15]+                              ; epilogue
	ldw %r9, [%r15]+                               ; epilogue
	ldw %r8, [%r15]+                               ; epilogue
	ldw %r7, [%r15]+                               ; epilogue
	ldw %r6, [%r15]+                               ; epilogue
	ldw %r5, [%r15]+                               ; epilogue
	ldw %r4, [%r15]+                               ; epilogue
	ret
.size ispu_fir_f32_small, .-ispu_fir_f32_small

