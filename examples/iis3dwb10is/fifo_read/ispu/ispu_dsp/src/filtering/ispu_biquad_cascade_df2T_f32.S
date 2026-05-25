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

.globl ispu_biquad_cascade_df2T_f32
.section .text.ispu_biquad_cascade_df2T_f32, "ax", @progbits
.type ispu_biquad_cascade_df2T_f32, @function
.align 4
ispu_biquad_cascade_df2T_f32:
	subw %r15, 4
	stw [%r15]-, %r4                                               ; prologue
	stw [%r15]-, %r5                                               ; prologue
	stw [%r15]-, %r6                                               ; prologue
	stw [%r15]-, %r7                                               ; prologue
	stw [%r15]-, %r8                                               ; prologue
	stw [%r15]-, %r9                                               ; prologue
	stw [%r15]-, %r10                                              ; prologue
	stw [%r15]-, %r11                                              ; prologue
	stw [%r15]-, %r12                                              ; prologue
	stw [%r15]-, %r14                                              ; prologue

	movw %r4, #0
	ldb %r4, [%r0]                                                 ; r4 = inst->num_stages
	addw %r0, #4
	ldw %r5, [%r0]+                                                ; r5 = inst->state
	ldw %r6, [%r0]                                                 ; r6 = inst->coeffs
	movw %r12, %r3                                                 ; r12 = block_size

	movw %r9, .ispu_biquad_cascade_df2T_f32.inner_stage_loop_start ; ISPU_LOOP_START = .ispu_biquad_cascade_df2T_f32.inner_loop_start
	stw [ISPU_LOOP_START], %r9
	movw %r9, .ispu_biquad_cascade_df2T_f32.inner_stage_loop_end   ; ISPU_LOOP_END = .ispu_biquad_cascade_df2T_f32.inner_loop_end
	stw [ISPU_LOOP_END], %r9

.ispu_biquad_cascade_df2T_f32.loop_stages_start:
	movw %r14, %r1                                                 ; r14 = src
	ldw %r1, [%r5]+                                                ; r1 = inst->state[stage][0] (d1)
	ldw %r11, [%r5]                                                ; r11 = inst->state[stage][1] (d2)
	stw [%r15]-, %r4                                               ; push inst->num_stages on the stack

.ispu_biquad_cascade_df2T_f32.stage_loop_start:
	movw %r7, %r3
	cmpw %r3, #0x400
	jpdlt .ispu_biquad_cascade_df2T_f32.load_stage_loop_cnt
	movw %r7, #0x400                                               ; r4 = block_size < 1024 ? block_size : 1024

.ispu_biquad_cascade_df2T_f32.load_stage_loop_cnt:
	subw %r7, #1                                                   ; ISPU_LOOP_CNT = 0x400 | (r4 - 1)
	bset %r7, #10
	stw [ISPU_LOOP_CNT], %r7
	stw [%r15]-, %r6                                               ; push &inst->coeffs[stage] on the stack
	stw [%r15], %r3                                                ; push block_size on the stack
	ldw %r7, [%r6]+                                                ; r7 = inst->coeffs[stage][0] (b0)
	ldw %r8, [%r6]+                                                ; r8 = inst->coeffs[stage][1] (b1)
	ldw %r3, [%r6]+                                                ; r3 = inst->coeffs[stage][2] (b2)
	ldw %r4, [%r6]+                                                ; r4 = inst->coeffs[stage][3] (a1)
	ldw %r6, [%r6]                                                 ; r6 = inst->coeffs[stage][4] (a2)

.ispu_biquad_cascade_df2T_f32.inner_stage_loop_start:
	ldw %r9, [%r14]+                                               ; r9 = src[i++] (x[n])
	movw %r0, #0                                                   ; r0 = 0
	acms %r9, %r7                                                  ; r0 += r1, r1 = b0 * x[n] (r0 = d1)
	acms %r9, %r8                                                  ; r0 += r1, r1 = b1 * x[n] (r0 = y[n] = b0 * x[n] + d1)
	stw [%r2]+, %r0                                                ; dst[i++] = y[n]
	movw %r10, %r0                                                 ; r10 = y[n]
	fmuls %r0, %r4                                                 ; r0 = a1 * y[n]
	acms %r9, %r3                                                  ; r0 += r1, r1 = b2 * x[n] (r0 = b1 * x[n] + a1 * y[n])
	fadds %r0, %r11                                                ; r0 = b1 * x[n] + a1 * y[n] + d2
	fmuls %r10, %r6                                                ; r10 = a2 * y[n]
	fadds %r10, %r1                                                ; r0 = b2 * x[n] + a2 * y[n] (d2 = b2 * x[n] + a2 * y[n])
	movw %r1, %r0                                                  ; r1 = b1 * x[n] + a1 * y[n] + d2 (d1 = b1 * x[n] + a1 * y[n] + d2)
.ispu_biquad_cascade_df2T_f32.inner_stage_loop_end:
	movw %r11, %r10                                                ; r11 = d2

	ldw %r3, [%r15]+                                               ; r3 = block_size
	subw %r3, #0x400                                               ; block_size -= 1024
	ldw %r6, [%r15]                                                ; r6 = &inst->coeffs[stage]
	cmpw %r3, #0x1
	jpdge .ispu_biquad_cascade_df2T_f32.stage_loop_start
.ispu_biquad_cascade_df2T_f32.stage_loop_end:

	addw %r15, #4
	ldw %r4, [%r15]                                                ; r4 = inst->num_stages
	stw [%r5]-, %r11                                               ; inst->state[stage][1] = d2
	stw [%r5], %r1                                                 ; inst->state[stage][1] = d1
	addw %r5, #8                                                   ; r5 = &inst->state[stage++]
	movw %r9, %r12                                                 ; r9 = block_size
	movw %r3, %r12                                                 ; r3 = block_size
	slaw %r9, #2                                                   ; r10 *= 4
	subw %r2, %r9                                                  ; r2 = dst
	movw %r1, %r2                                                  ; r1 = dst (input of the next stage is the output of the previous)
	subb %r4, #1                                                   ; num_stages -= 1
	cmpb %r4, #0                                                   ; if num_stages > 0, jump to loop_stages_start
	addw %r6, #20                                                  ; r6 = inst->coeffs[stage++]
	jpdne .ispu_biquad_cascade_df2T_f32.loop_stages_start
.ispu_biquad_cascade_df2T_f32.loop_stages_end:

	ldw %r4, [%r15]+                                               ; clear the stack
	ldw %r14, [%r15]+                                              ; epilogue
	ldw %r12, [%r15]+                                              ; epilogue
	ldw %r11, [%r15]+                                              ; epilogue
	ldw %r10, [%r15]+                                              ; epilogue
	ldw %r9, [%r15]+                                               ; epilogue
	ldw %r8, [%r15]+                                               ; epilogue
	ldw %r7, [%r15]+                                               ; epilogue
	ldw %r6, [%r15]+                                               ; epilogue
	ldw %r5, [%r15]+                                               ; epilogue
	ldw %r4, [%r15]+                                               ; epilogue
	ret
.size ispu_biquad_cascade_df2T_f32, .-ispu_biquad_cascade_df2T_f32

