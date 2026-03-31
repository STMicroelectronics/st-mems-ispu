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

.globl l1_norm
.section .text.l1_norm, "ax", @progbits
.type l1_norm, @function
.align 4
l1_norm:
	subw %r15, #4              ; prologue
	stw [%r15], %r2            ; prologue

	movw %r2, loop_start       ; ISPU_LOOP_START = loop_start
	stw [ISPU_LOOP_START], %r2
	movw %r2, loop_end         ; ISPU_LOOP_END = loop_end
	stw [ISPU_LOOP_END], %r2
	subw %r1, #1               ; ISPU_LOOP_CNT = len - 1
	bset %r1, #10              ; ISPU_LOOP_CNT[10] = 1 (LOOP_EN = 1)
	stw [ISPU_LOOP_CNT], %r1

	movw %r1, #0               ; r1 = 0.0f
loop_start:
	ldw %r2, [%r0]+            ; r2 = buff[i++]
	bclear %r2, #0x1F          ; r2 = fabsf(r2)
loop_end:
	fadds %r1, %r2             ; r1 += r2

	movw %r0, %r1              ; r0 = r1
	ldw %r2, [%r15]+           ; epilogue
	ret                        ; return norm on r0
.size l1_norm, .-l1_norm

