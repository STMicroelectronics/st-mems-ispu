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

.globl ispu_biquad_cascade_df1_f32
.section .text.ispu_biquad_cascade_df1_f32, "ax", @progbits
.type ispu_biquad_cascade_df1_f32, @function
.align 4
ispu_biquad_cascade_df1_f32:
	subw %r15, 4
	stw [%r15]-, %r4                                              ; prologue
	stw [%r15]-, %r5                                              ; prologue
	stw [%r15]-, %r6                                              ; prologue
	stw [%r15]-, %r7                                              ; prologue
	stw [%r15]-, %r8                                              ; prologue
	stw [%r15]-, %r9                                              ; prologue
	stw [%r15]-, %r10                                             ; prologue
	stw [%r15]-, %r11                                             ; prologue
	stw [%r15]-, %r12                                             ; prologue
	stw [%r15]-, %r14                                             ; prologue

	movw %r4, #0
	ldb %r4, [%r0]                                                ; r4 = inst->num_stages
	addw %r0, #4
	ldw %r5, [%r0]+                                               ; r5 = inst->state
	ldw %r6, [%r0]                                                ; r6 = inst->coeffs
	stw [%r15]-, %r3                                              ; push block_size on the stack

	movw %r9, .ispu_biquad_cascade_df1_f32.inner_stage_loop_start ; ISPU_LOOP_START = .ispu_biquad_cascade_df1_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r9
	movw %r9, .ispu_biquad_cascade_df1_f32.inner_stage_loop_end   ; ISPU_LOOP_END = .ispu_biquad_cascade_df1_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r9

.ispu_biquad_cascade_df1_f32.loop_stages_start:
	ldw %r10, [%r5]+                                              ; r10 = inst->state[stage][0] (x[n - 1])
	ldw %r11, [%r5]+                                              ; r11 = inst->state[stage][1] (x[n - 2])
	ldw %r12, [%r5]+                                              ; r12 = inst->state[stage][2] (y[n - 1])
	ldw %r14, [%r5]                                               ; r14 = inst->state[stage][3] (y[n - 2])
	subw %r5, #12
	stw [%r15]-, %r5                                              ; push &inst->state[stage] on the stack
	movw %r5, %r1                                                 ; r5 = src
	stw [%r15]-, %r4                                              ; push inst->num_stages on the stack

.ispu_biquad_cascade_df1_f32.stage_loop_start:
	movw %r7, %r3
	cmpw %r3, #0x400
	jpdlt .ispu_biquad_cascade_df1_f32.load_stage_loop_cnt
	movw %r7, #0x400                                              ; r4 = block_size < 1024 ? block_size : 1024

.ispu_biquad_cascade_df1_f32.load_stage_loop_cnt:
	subw %r7, #1                                                  ; ISPU_LOOP_CNT = 0x400 | (r4 - 1)
	bset %r7, #10
	stw [ISPU_LOOP_CNT], %r7
	stw [%r15]-, %r6                                              ; push &inst->coeffs[stage] on the stack
	stw [%r15], %r3                                               ; push block_size on the stack
	ldw %r7, [%r6]+                                               ; r7 = inst->coeffs[stage][0] (b0)
	ldw %r8, [%r6]+                                               ; r8 = inst->coeffs[stage][1] (b1)
	ldw %r3, [%r6]+                                               ; r3 = inst->coeffs[stage][2] (b2)
	ldw %r4, [%r6]+                                               ; r4 = inst->coeffs[stage][3] (a1)
	ldw %r6, [%r6]                                                ; r6 = inst->coeffs[stage][4] (a2)

.ispu_biquad_cascade_df1_f32.inner_stage_loop_start:
	movw %r1, #0                                                  ; r1 = 0
	ldw %r9, [%r5]+                                               ; r9 = src[i++] (x[n])
	movw %r0, #0                                                  ; r0 = 0
	acms %r9, %r7                                                 ; r0 += r1, r1 = b0 * x[n]
	acms %r10, %r8                                                ; r0 += r1, r1 = b1 * x[n - 1]
	acms %r11, %r3                                                ; r0 += r1, r1 = b2 * x[n - 2]
	acms %r12, %r4                                                ; r0 += r1, r1 = a1 * y[n - 1]
	acms %r14, %r6                                                ; r0 += r1, r1 = a2 * y[n - 2]
	fadds %r0, %r1                                                ; r0 += r1 (last accumulate)
	movw %r14, %r12                                               ; y[n - 2] = y[n - 1]
	movw %r11, %r10                                               ; x[n - 2] = x[n - 1]
	movw %r10, %r9                                                ; x[n - 1] = x[n]
	stw [%r2]+, %r0                                               ; dst[i++] = y[n]
.ispu_biquad_cascade_df1_f32.inner_stage_loop_end:
	movw %r12, %r0                                                ; y[n - 1] = y[n]

	ldw %r3, [%r15]+                                              ; r3 = block_size
	subw %r3, #0x400                                              ; block_size -= 1024
	ldw %r6, [%r15]                                               ; r6 = &inst->coeffs[stage]
	cmpw %r3, #0x1
	jpdge .ispu_biquad_cascade_df1_f32.stage_loop_start
.ispu_biquad_cascade_df1_f32.stage_loop_end:

	addw %r15, #4
	ldw %r4, [%r15]+                                              ; r4 = inst->num_stages
	ldw %r5, [%r15]                                               ; r5 = &inst->state[stage][3]
	stw [%r5]+, %r10                                              ; inst->state[0] = src[block_size - 1]
	stw [%r5]+, %r11                                              ; inst->state[1] = src[block_size - 2]
	stw [%r5]+, %r12                                              ; inst->state[2] = dst[block_size - 1]
	stw [%r5]+, %r14                                              ; inst->state[3] = dst[block_size - 2]
	movw %r7, %r15                                                ; r7 = stack pointer
	addw %r7, #4                                                  ; r7 += 4
	ldw %r9, [%r7]                                                ; r9 = block_size
	movw %r3, %r9                                                 ; r3 = block_size
	slaw %r9, #2                                                  ; r10 *= 4
	subw %r2, %r9                                                 ; r2 = dst
	movw %r1, %r2                                                 ; r1 = dst (input of the next stage is the output of the previous)
	subb %r4, #1                                                  ; num_stages -= 1
	cmpb %r4, #0                                                  ; if num_stages > 0, jump to loop_stages_start
	addw %r6, #20                                                 ; r6 = inst->coeffs[stage++]
	jpdne .ispu_biquad_cascade_df1_f32.loop_stages_start
.ispu_biquad_cascade_df1_f32.loop_stages_end:

	ldw %r5, [%r15]+
	ldw %r3, [%r15]+
	ldw %r14, [%r15]+                                             ; epilogue
	ldw %r12, [%r15]+                                             ; epilogue
	ldw %r11, [%r15]+                                             ; epilogue
	ldw %r10, [%r15]+                                             ; epilogue
	ldw %r9, [%r15]+                                              ; epilogue
	ldw %r8, [%r15]+                                              ; epilogue
	ldw %r7, [%r15]+                                              ; epilogue
	ldw %r6, [%r15]+                                              ; epilogue
	ldw %r5, [%r15]+                                              ; epilogue
	ldw %r4, [%r15]+                                              ; epilogue
	ret
.size ispu_biquad_cascade_df1_f32, .-ispu_biquad_cascade_df1_f32

