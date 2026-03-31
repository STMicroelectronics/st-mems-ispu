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

.globl ispu_rfft_f32
.section .text.ispu_rfft_f32, "ax", @progbits
.type ispu_rfft_f32, @function
.align 4
ispu_rfft_f32:
	subw %r15, 4
	stw [%r15]-, %r4                            ; prologue
	stw [%r15]-, %r5                            ; prologue
	stw [%r15]-, %r6                            ; prologue
	stw [%r15]-, %r7                            ; prologue
	stw [%r15]-, %r8                            ; prologue
	stw [%r15]-, %r9                            ; prologue
	stw [%r15]-, %r10                           ; prologue
	stw [%r15]-, %r11                           ; prologue
	stw [%r15]-, %r12                           ; prologue
	stw [%r15]-, %r14                           ; prologue
	ldw %r14, [%r0]+                            ; r14 = inst->twiddle_table
	stw [%r15]-, %r14                           ; save inst->twiddle_table to the stack
	stw [%r15], %r1                             ; save data to the stack

	movw %r14, #0
	ldh %r14, [%r0]                             ; r14 = inst->fft_len
	addw %r0, #2
	movw %r9, #0
	ldb %r9, [%r0]                              ; r9 = inst->fft_n_elem_bra
	stw [ISPU_FFT_N_ELEM_BRA], %r9

	movp %r11, %r1                              ; r11 = data
	addw %r11, #8                               ; r11 = &data[2]. First complex number does not need bit reversing
	stw [ISPU_FFT_CURR_BRA], %r11               ; ISPU_FFT_CURR_BRA = &data[2]

	movp %r10, ISPU_FFT_MATCH_BRA               ; r10 = ISPU_FFT_MATCH_BRA

	movp %r12, .ispu_rfft_f32.loop_bra_start
	stw [ISPU_LOOP_START], %r12
	movp %r12, .ispu_rfft_f32.loop_bra_end
	stw [ISPU_LOOP_END], %r12
	movw %r12, %r14                             ; r12 = fft_len / 2 - 3. First and last complex numbers do not need bit reversing
	sraw %r12, 1
	subw %r12, #3
	bset %r12, #10
	stw [ISPU_LOOP_CNT], %r12

.ispu_rfft_f32.loop_bra_start:                  ; Bit Reverse Addressing stage: i index of complex numbers
                                                ; RA = Reversed Address
	ldw %r0, [%r10]                             ; r0 = Re(x(i))
	ldw %r1, [%r10]                             ; r1 = Re(x(RA(i)))
	stw [%r10], %r0                             ; swap Re(x(i)) with Re(x(Ra(i)))
	stw [%r10], %r1

	ldw %r0, [%r10]                             ; r0 = Im(x(i))
	ldw %r1, [%r10]                             ; r1 = Im(x(RA(i)))
	stw [%r10], %r0                             ; swap Im(x(i)) with Im(x(RA(i)))
.ispu_rfft_f32.loop_bra_end:
	stw [%r10], %r1

	movp %r12, .ispu_rfft_f32.loop_stage1_start
	stw [ISPU_LOOP_START], %r12                 ; set start address
	movp %r12, .ispu_rfft_f32.loop_stage1_end
	stw [ISPU_LOOP_END], %r12                   ; set end address
	movw %r12, %r14                             ; r12 = fft_len / 4 - 1
	sraw %r12, #2
	subw %r12, #1
	bset %r12, #10
	stw [ISPU_LOOP_CNT], %r12

	ldw %r11, [%r15]                            ; load data from stack
	movw %r10, %r11

.ispu_rfft_f32.loop_stage1_start:               ; stage 1: twiddle factor is 1 + 0j (trivial)
	ldw %r0, [%r11]+                            ; r0 = Re(x(i))
	movw %r4, %r0                               ; r4 = Re(x(i))
	ldw %r1, [%r11]+                            ; r1 = Im(x(i))
	movw %r5, %r1                               ; r5 = Im(x(i))
	ldw %r2, [%r11]+                            ; r2 = Re(x(i + 1))
	fadds %r0, %r2                              ; r0 = Re(x(i)) + Re(x(i + 1))
	ldw %r3, [%r11]+                            ; r3 = Im(x(i + 1))

	fadds %r1, %r3                              ; r1 = Im(x(i)) + Im(x(i + 1))
	stw [%r10]+, %r0                            ; Re(y(i)) = Re(x(i)) + Re(x(i + 1))

	fsubs %r4, %r2                              ; r4 = Re(x(i)) - Re(x(i + 1))
	stw [%r10]+, %r1                            ; Im(y(i)) = Im(x(i)) + Im(x(i + 1))

	fsubs %r5, %r3                              ; r5 = Im(x(i)) - Im(x(i + 1))
	stw [%r10]+, %r4                            ; Re(y(i + 1)) = Re(x(i)) - Re(x(i + 1))

.ispu_rfft_f32.loop_stage1_end:
	stw [%r10]+, %r5                            ; Re(y(i + 1)) = Im(x(i)) - Im(x(i + 1))

	movp %r12, .ispu_rfft_f32.loop_stage2_start
	stw [ISPU_LOOP_START], %r12
	movp %r12, .ispu_rfft_f32.loop_stage2_end
	stw [ISPU_LOOP_END], %r12
	movw %r12, %r14                             ; r12 = fft_len / 8 - 1
	sraw %r12, #3
	subw %r12, #1
	bset %r12, #10
	stw [ISPU_LOOP_CNT], %r12

	movw %r10, ISPU_FFT_MATCH_D                 ; r10 = ISPU_FFT_MATCH_D (register for fetching data in butterfly scheme)

	movbi %r8, 0x2
	stw [ISPU_FFT_TWIDDLE], %r8                 ; ISPU_FFT_TWIDDLE = 2 (number of twiddles)
	ldw %r0, [%r15]                             ; load data from stack
	stw [ISPU_FFT_CURR_D1], %r0                 ; ISPU_FFT_CURR_D1 = &data[0]

.ispu_rfft_f32.loop_stage2_start:               ; stage 2: twiddle factors are 1 + 0j, 0 - 1j (trivial)
                                                ; d1, d2 first butterfly input pair. p1, p2 first butterfly output pair
	ldw %r0, [%r10]                             ; r0 = Re(d1)
	ldw %r1, [%r10]                             ; r1 = Im(d1)
	ldw %r2, [%r10]                             ; r2 = Re(d2)
	ldw %r3, [%r10]                             ; r3 = Im(d2)

	movw %r4, %r0                               ; r4 = Re(d1)
	fadds %r0, %r2                              ; r0 = Re(d1) + Re(d2)
	movw %r5, %r1                               ; r5 = Im(d1)
	stw [%r10], %r0                             ; Re(p1) = Re(d1) + Re(d2)

	fadds %r1, %r3                              ; r1 = Im(d1) + Im(d2)
	fsubs %r4, %r2                              ; r4 = Re(d1) - Re(d2)
	stw [%r10], %r1                             ; Im(p1) = Im(d1) + Im(d2)

	fsubs %r5, %r3                              ; r5 = Im(d1) - Im(d2)
	stw [%r10], %r4                             ; Re(p2) = Re(d1) - Re(d2)
	stw [%r10], %r5                             ; Im(p2) = Im(d1) - Im(d2)

                                                ; d1, d2 second butterfly input pair. p1, p2 second butterfly output pair
	ldw %r0, [%r10]                             ; r0 = Re(d1)
	ldw %r1, [%r10]                             ; r1 = Im(d1)
	ldw %r2, [%r10]                             ; r2 = Re(d2)
	ldw %r3, [%r10]                             ; r3 = Im(d2)

	movw %r4, %r0                               ; r4 = Re(d1)
	fadds %r0, %r3                              ; r0 = Re(d1) + Im(d2)
	movw %r5, %r1                               ; r5 = Im(d1)
	stw [%r10], %r0                             ; Re(p1) = Re(d1) + Im(d2)

	fsubs %r1, %r2                              ; r1 = Im(d1) - Re(d2)
	fsubs %r4, %r3                              ; r4 = Re(d1) - Im(d2)
	stw [%r10], %r1                             ; Im(p1) = Im(d1) - Re(d2)

	fadds %r5, %r2                              ; r5 = Im(d1) + Re(d2)
	stw [%r10], %r4                             ; Re(p2) = Re(d1) - Im(d2)

.ispu_rfft_f32.loop_stage2_end:
	stw [%r10], %r5                             ; Im(p2) = Im(d1) + Re(d2)

	movw %r11, %r14                             ; r11 = inst->fft_len / 8
	sraw %r11, #3
	movbi %r8, 0x4                              ; init number of twiddle factors to 4
	ldw %r9, [ISPU_FFT_N_ELEM_BRA]              ; r9 = n_elem_bra
	stw [ISPU_FFT_STRIDE_TW], %r11              ; init twiddle stride to inst->fft_len / 8
	addw %r9, #2                                ; r9 = inst->n_elem_bra + 2 (number of stages left)

.ispu_rfft_f32.loop_stages_start:
	movw %r12, %r15                             ; r12 = inst->twiddle_table (from the stack)
	addw %r12, #4
	ldw %r12, [%r12]
	movw %r11, ISPU_FFT_MATCH_TW                ; r11 = ISPU_FFT_MATCH_TW (register for fetching twiddle factor in butterfly scheme)
	movp %r0, %r12                              ; r0 = inst->twiddle_table
	stw [ISPU_FFT_START_TW], %r0                ; ISPU_FFT_START_TW = inst->twiddle_table
	stw [ISPU_FFT_CURR_TW1], %r0                ; ISPU_FFT_CURR_TW1 = inst->twiddle_table (pointer to the real part of twiddle factors)

	movw %r10, %r14                             ; r10 = r14 (inst->fft_len)
	addw %r10, %r12                             ; r10 = inst->twiddle_table + inst->fft_len (twiddle_table size in bytes corresponds to fft_len)
	movp %r0, %r10                              ; r0 = twiddle_table_end
	movw %r10, ISPU_FFT_MATCH_D                 ; r10 = ISPU_FFT_MATCH_D (register for fetching data in butterfly scheme)
	stw [ISPU_FFT_END_TW], %r0                  ; ISPU_FFT_END_TW = twiddle_table_end
	stw [ISPU_FFT_CURR_TW2], %r0                ; ISPU_FFT_CURR_TW2 = twiddle_table_end (pointer to imag part of twiddle factors)

	stw [ISPU_FFT_TWIDDLE], %r8                 ; ISPU_FFT_TWIDDLE = r8 (number of twiddle factors)
	ldw %r0, [%r15]                             ; load data from stack

	movp %r12, .ispu_rfft_f32.loop_stage_start
	stw [ISPU_FFT_CURR_D1], %r0                 ; ISPU_FFT_CURR_D1 = data
	stw [ISPU_LOOP_START], %r12
	movp %r12, .ispu_rfft_f32.loop_stage_end
	movw %r2, %r14                              ; r2 = inst->fft_len / 4 - 1
	sraw %r2, #2
	subw %r2, #1
	bset %r2, #10
	stw [ISPU_LOOP_END], %r12
	stw [ISPU_LOOP_CNT], %r2

.ispu_rfft_f32.loop_stage_start:                ; remaining stages: w is the twiddle factor
                                                ; d1, d2 first butterfly input pair. p1, p2 first butterfly output pair
	ldw %r2, [%r10]                             ; r2 = Re(d1)
	ldw %r3, [%r10]                             ; r3 = Im(d1)

	ldw %r0, [%r11]                             ; r0 = Re(w)
	ldw %r1, [%r11]                             ; r1 = Im(w)

	ldw %r4, [%r10]                             ; r4 = Re(d2)
	movw %r5, %r4                               ; r5 = Re(d2)
	ldw %r6, [%r10]                             ; r6 = Im(d2)
	movw %r7, %r6                               ; r7 = Im(d2)

	fmuls %r5, %r0                              ; r5 = Re(d2) * Re(w)
	fmuls %r7, %r1                              ; r7 = Im(d2) * Im(w)
	fsubs %r5, %r7                              ; r5 = Re(d2) * Re(w) - Im(d2) * Im(w)

	fmuls %r4, %r1                              ; r4 = Re(d2) * Im(w)
	fmuls %r6, %r0                              ; r6 = Im(d2) * Re(w)

	movw %r0, %r2                               ; r0 = Re(d1)
	fadds %r0, %r5                              ; r0 = Re(d1) + Re(d2) * Re(w) - Im(d2) * Im(w)

	fadds %r4, %r6                              ; r4 = Re(d2) * Im(w) + Im(d2) * Re(w)
	stw [%r10], %r0                             ; Re(p1) = Re(d1) + Re(d2) * Re(w) - Im(d2) * Im(w)

	movw %r0, %r3                               ; r0 = Im(d1)
	fadds %r0, %r4                              ; r0 = Im(d1) + Re(d2) * Im(w) + Im(d2) * Re(w)
	fsubs %r2, %r5                              ; r2 = Re(d1) - Re(d2) * Re(w) + Im(d2) * Im(w)
	stw [%r10], %r0                             ; Im(p1) = Im(d1) + Re(d2) * Im(w) + Im(d2) * Re(w)

	fsubs %r3, %r4                              ; r3 = Im(d1) - Re(d2) * Im(w) - Im(d2) * Re(w)
	stw [%r10], %r2                             ; Re(p2) = Re(d1) - Re(d2) * Re(w) + Im(d2) * Im(w)

.ispu_rfft_f32.loop_stage_end:
	stw [%r10], %r3                             ; Im(p2) = Im(d1) - Re(d2) * Im(w) - Im(d2) * Re(w)

	ldw %r12, [ISPU_FFT_STRIDE_TW]
	srlw %r12, 0x1                              ; r12 = twiddle stride / 2
	subw %r9, 0x1                               ; decrement number of stages left
	stw [ISPU_FFT_STRIDE_TW], %r12              ; ISPU_FFT_STRIDE_TW = r12

	cmpw %r9, 0x0
	slaw %r8, 0x1                               ; r8 = number of twiddle factors * 2
	jpdne .ispu_rfft_f32.loop_stages_start
.ispu_rfft_f32.loop_stages_end:

	movw %r11, %r15                             ; r11 = inst->twiddle_table (from the stack)
	addw %r11, #4
	ldw %r11, [%r11]
	movp %r0, %r11
	stw [ISPU_FFT_START_TW], %r0                ; ISPU_FFT_START_TW = inst->twiddle_table
	movp %r1, %r0
	addw %r1, %r14
	stw [ISPU_FFT_CURR_TW1], %r0                ; ISPU_FFT_CURR_TW1 = inst->twiddle_table (pointer to the imag part of twiddle factors)

	movh %r0, 0x201
	stw [ISPU_FFT_END_TW], %r1                  ; ISPU_FFT_END_TW = inst->twiddle_table + inst->fft_len (twiddle_table size in bytes corresponds to fft_len)
	stw [ISPU_FFT_CURR_TW2], %r1                ; ISPU_FFT_CURR_TW1 = inst->twiddle_table (pointer to the real part of twiddle factors)
	stw [ISPU_FFT_STRIDE_TW], %r0               ; ISPU_FFT_STRIDE_TW = 0x201 (stride 1 and bit 9 to 1 for the split stage)

	movp %r11, ISPU_FFT_MATCH_TW                ; twiddle pointer
	ldw %r0, [%r15]                             ; load data from stack
	movw %r1, %r0
	movw %r2, %r14                              ; r2 = (inst->fft_len * 4) - 8
	slaw %r2, #2
	subw %r2, #8
	addw %r1, %r2

	ldw %r2, [%r0]+                             ; r2 = Re(XA(0))
	ldw %r3, [%r0]                              ; r3 = Im(XA(0))
	ldw %r5, [%r11]                             ; r5 = Im(w0) (move forward twiddle factor pointer)
	ldw %r6, [%r11]                             ; r6 = Re(w0)

	movw %r4, %r2
	fsubs %r2, %r3                              ; r2 = Re(X(0)) - Im(X(0))
	fadds %r3, %r4                              ; r3 = Re(X(0)) + Im(X(0))
	stw [%r0]-, %r2                             ; Im(X(0)) = Re(X(0)) - Im(X(0))
	stw [%r0], %r3                              ; Re(X(0)) = Re(X(0)) + Im(X(0))
	addw %r0, 0x8                               ; r0 = &data[1]

	movp %r12, .ispu_rfft_f32.loop_split_start
	movp %r2, .ispu_rfft_f32.loop_split_end
	stw [ISPU_LOOP_START], %r12
	movw %r12, %r14                             ; r12 = inst->fft_len / 4 - 2
	sraw %r12, #2
	subw %r12, #2
	bset %r12, #10
	stw [ISPU_LOOP_END], %r2
	stw [ISPU_LOOP_CNT], %r12

	movw %r12, 0x3f000000                       ; r12 = 0.5

.ispu_rfft_f32.loop_split_start:                ; split stage: reconstruct real fft
                                                ; N = inst->fft_len / 2, i from 1 to N / 2 - 1 index of complex numbers, k = N - i
	ldw %r2, [%r0]+                             ; r2 = Re(XA(i))
	ldw %r3, [%r1]+                             ; r3 = Re(XA(k))
	movw %r9, %r3                               ; r9 = Re(XA(k))
	fadds %r9, %r2                              ; r9 = Re(XA(i)) + Re(XA(k))
	fsubs %r3, %r2                              ; r3 = Re(XA(k)) - Re(XA(i))

	ldw %r5, [%r11]                             ; r5 = Im(w(i))
	ldw %r6, [%r11]                             ; r6 = Re(w(i))

	movw %r7, %r3
	fmuls %r7, %r5                              ; r7 = F1 = Im(w(i)) * (Re(XA(k)) - Re(XA(i)))
	fmuls %r3, %r6                              ; r3 = F2 = Re(w(i)) * (Re(XA(k)) - Re(XA(i)))

	ldw %r2, [%r0]                              ; r2 = Im(XA(i))
	ldw %r4, [%r1]                              ; r4 = Im(XA(k))

	movw %r8, %r2
	fsubs %r8, %r4                              ; r8 = Im(XA(i)) - Im(XA(k))
	fadds %r2, %r4                              ; r2 = Im(XA(i)) + Im(XA(k))

	fmuls %r6, %r2                              ; r6 = F3 = Re(w(i)) * (Im(XA(k)) + Im(XA(i)))
	fmuls %r2, %r5                              ; r2 = F4 = Im(w(i)) * (Im(XA(k)) + Im(XA(i)))

	fadds %r3, %r2                              ; r3 = F2 + F4
	fsubs %r7, %r6                              ; r7 = F1 - F3

	movw %r2, %r3                               ; r2 = F2 + F4
	movw %r6, %r7                               ; r6 = F1 - F3

	fadds %r2, %r9                              ; r2 = F2 + F4 + Re(XA(i)) + Re(XA(k))
	fmuls %r2, %r12                             ; r2 = 0.5 * (F2 + F4 + Re(XA(i)) + Re(XA(k)))

	fadds %r6, %r8                              ; r6 = F1 - F3 + Im(XA(i)) - Im(XA(k))

	fmuls %r6, %r12                             ; r6 = 0.5 * (F1 - F3 + Im(XA(i)) - Im(XA(k)))
	fsubs %r9, %r3                              ; r9 = Re(XA(i)) + Re(XA(k)) - F2 - F4
	stw [%r0]-, %r6                             ; Im(X(i)) = 0.5 * (F1 - F3 + Im(XA(i)) - Im(XA(k)))

	fmuls %r9, %r12                             ; r9 = 0.5 * (Re(XA(i)) + Re(XA(k)) - F2 - F4)
	stw [%r0], %r2                              ; Re(X(i)) = 0.5 * (F2 + F4 + Re(XA(i)) + Re(XA(k)))

	fsubs %r8, %r7                              ; r8 = Im(XA(i)) - Im(XA(k)) - F1 + F3

	fmuls %r8, %r12                             ; r8 = -0.5 * (Im(XA(i)) - Im(XA(k)) - F1 + F3)
	binv %r8, #31
	stw [%r1]-, %r8                             ; Im(X(k)) = -0.5 * (Im(XA(i)) - Im(XA(k)) - F1 + F3)

	addw %r0, 0x8
	stw [%r1], %r9                              ; Re(X(k)) = 0.5 * (Re(XA(i)) + Re(XA(k)) - F2 - F4)

.ispu_rfft_f32.loop_split_end:
	subw %r1, 0x8

	addw %r0, 0x4
	ldw %r2, [%r0]                              ; r2 = Im(X(N - 1))
	binv %r2, #31                               ; r2 = -Im(X(N - 1))

	stw [%r0], %r2                              ; Im(X(N - 1)) = -Im(X(N - 1))

	ldw %r1, [%r15]+                            ; clear stack
	ldw %r0, [%r15]+                            ; clear stack
	ldw %r14, [%r15]+                           ; epilogue
	ldw %r12, [%r15]+                           ; epilogue
	ldw %r11, [%r15]+                           ; epilogue
	ldw %r10, [%r15]+                           ; epilogue
	ldw %r9, [%r15]+                            ; epilogue
	ldw %r8, [%r15]+                            ; epilogue
	ldw %r7, [%r15]+                            ; epilogue
	ldw %r6, [%r15]+                            ; epilogue
	ldw %r5, [%r15]+                            ; epilogue
	ldw %r4, [%r15]+                            ; epilogue
	ret
.size ispu_rfft_f32, .-ispu_rfft_f32

