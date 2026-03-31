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

.globl dot
.section .text.dot, "ax", @progbits
.type dot, @function
.align 4
dot:
	subw %r15, #4    ; prologue
	stw [%r15]-, %r3 ; prologue
	stw [%r15]-, %r4 ; prologue
	stw [%r15]-, %r5 ; prologue
	stw [%r15], %r6  ; prologue

	movw %r3, %r0    ; r3 = r0 (r0 will be used by acms)
	movw %r4, %r1    ; r4 = r1 (r1 will be used by acms)
	movw %r0, #0     ; r0 = 0.0f
	movw %r1, #0     ; r1 = 0.0f
loop_start:
	ldw %r5, [%r3]+  ; r5 = a[i++]
	ldw %r6, [%r4]+  ; r6 = b[i++]
	acms %r5, %r6    ; r0 += r1 ; r1 = r5 * r6
	subh %r2, #1     ; r2 = len--
	cmph %r2, #0     ; if r2 != 0, jump to loop_start
	jpdne loop_start
loop_end:
	fadds %r0, %r1   ; r0 += r1

	ldw %r6, [%r15]+ ; epilogue
	ldw %r5, [%r15]+ ; epilogue
	ldw %r4, [%r15]+ ; epilogue
	ldw %r3, [%r15]+ ; epilogue

	ret              ; return dot product on r0
.size dot, .-dot

