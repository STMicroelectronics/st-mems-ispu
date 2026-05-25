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

.globl ispu_fir_f32_big
.section .text.ispu_fir_f32_big, "ax", @progbits
.type ispu_fir_f32_big, @function
.align 4
ispu_fir_f32_big:
	subw %r15, 4
	stw [%r15]-, %r4                             ; prologue
	stw [%r15]-, %r5                             ; prologue
	stw [%r15]-, %r6                             ; prologue
	stw [%r15]-, %r7                             ; prologue
	stw [%r15]-, %r8                             ; prologue
	stw [%r15]-, %r9                             ; prologue
	stw [%r15]-, %r10                            ; prologue
	stw [%r15]-, %r11                            ; prologue
	stw [%r15]-, %r12                            ; prologue
	stw [%r15]-, %r14                            ; prolgoue

	movw %r11, %r1                               ; r11 = src
	movw %r9, #0
	ldh %r9, [%r0]                               ; r9 = num_taps
	subw %r9, #1                                 ; r9 -= 1
	addw %r0, #4
	ldw %r4, [%r0]+                              ; r4 = state
	subw %r3, #1                                 ; r3 = block_size - 1
	stw [%r15]-, %r4                             ; save state to the stack
	ldw %r5, [%r0]+                              ; r5 = coeffs

	movw %r6, .ispu_fir_f32_big.inner_loop_start
	stw [ISPU_LOOP_START], %r6
	movw %r6, .ispu_fir_f32_big.inner_loop_end
	stw [ISPU_LOOP_END], %r6

	movw %r6, %r9
	slaw %r6, #2
	addw %r9, #1
	stw [%r15], %r9                              ; save num_taps - 1 to the stack
	addw %r6, %r4                                ; r6 = &state[num_taps - 1]

.ispu_fir_f32_big.loop_start:
	ldw %r12, [%r11]+                            ; r12 = src[i++]
	movw %r7, %r4                                ; r7 = r4
	stw [%r6]+, %r12                             ; state[i + num_taps - 1] = r12 (load next input sample into state buffer)
	movw %r8, %r5                                ; r8 = coeffs
	movw %r1, 0x0
	movw %r0, 0x0

.ispu_fir_f32_big.inner_loop_reload:
	cmpw %r9, #0x400
	movw %r10, %r9
	jpdlt .ispu_fir_f32_big.load_loop_cnt
	movw %r10, #0x400                            ; r3 = num_taps < 1024 ? block_size : 1024

.ispu_fir_f32_big.load_loop_cnt:
	subw %r10, #1
	bset %r10, #10
	stw [ISPU_LOOP_CNT], %r10

.ispu_fir_f32_big.inner_loop_start:              ; accumulate products between filter and input mask in r0
	ldw %r10, [%r7]+
	ldw %r14, [%r8]+
.ispu_fir_f32_big.inner_loop_end:
	acms %r10, %r14

	subw %r9, #0x400                             ; block_size -= 1024
	cmpw %r9, #0x1
	jpdge .ispu_fir_f32_big.inner_loop_reload

	fadds %r0, %r1                               ; add last results
	stw [%r2]+, %r0                              ; dst[i++] = r0
	ldw %r9, [%r15]                              ; r9 = num_taps (reload from the stack)
	addw %r4, #4                                 ; move input mask on
	cmpw %r3, #0
	subw %r3, #1
	jpdne .ispu_fir_f32_big.loop_start           ; loop for all the elements in the block

	addw %r15, #4
	ldw	%r6, [%r15]+                             ; r6 = state
	movw %r7, .ispu_fir_f32_big.copy_state_start
	stw [ISPU_LOOP_START], %r7
	movw %r7, .ispu_fir_f32_big.copy_state_end
	stw [ISPU_LOOP_END], %r7

	subw %r9, #1                                 ; r9 = inst->num_taps - 1
.ispu_fir_f32_big.copy_reload:
	cmpw %r9, #0x400
	movw %r10, %r9
	jpdlt .ispu_fir_f32_big.load_copy_cnt
	movw %r10, #0x400                            ; r10 = inst-> num_taps < 1024 ? inst->num_taps - 1 : 1024

.ispu_fir_f32_big.load_copy_cnt:
	subw %r10, #1
	bset %r10, #10
	stw [ISPU_LOOP_CNT], %r10

.ispu_fir_f32_big.copy_state_start:              ; copy the last num_taps - 1 input samples at state
	ldw %r11, [%r4]+
.ispu_fir_f32_big.copy_state_end:
	stw [%r6]+, %r11

	subw %r9, #0x400                             ; r9 -= 1024
	cmpw %r9, #0x1
	jpdge .ispu_fir_f32_big.copy_reload


	ldw %r14, [%r15]+                            ; epilogue
	ldw %r12, [%r15]+                            ; epilogue
	ldw %r11, [%r15]+                            ; epilogue
	ldw %r10, [%r15]+                            ; epilogue
	ldw %r9, [%r15]+                             ; epilogue
	ldw %r8, [%r15]+                             ; epilogue
	ldw %r7, [%r15]+                             ; epilogue
	ldw %r6, [%r15]+                             ; epilogue
	ldw %r5, [%r15]+                             ; epilogue
	ldw %r4, [%r15]+                             ; epilogue
	ret
.size ispu_fir_f32_big, .-ispu_fir_f32_big

