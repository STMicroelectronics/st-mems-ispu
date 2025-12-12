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

.globl ispu_sint_to_float
.section .text.ispu_sint_to_float,"ax",@progbits
.type ispu_sint_to_float, @function
; convert a signed integer %r0 into a float %r0 (used r1)
.align 4
ispu_sint_to_float:
        addw   %r0, #2147483648 ; add int value 2^31, shift value (0: 4294967295)(0:2^32)
        movw   %r1, %r0         ; lower 16 bits
        uexth  %r1, %r1
        orw    %r1, 0x4b000000  ; exp 2^23
        srlw   %r0, #16
        orw    %r0, 0x53000000  ; exp 2^39 (2^23 * 2^16)
        fsubs  %r0, 0x53008080  ; sub float (551911686144) (2^23 * 2^16) + shift down to 0 (-2^31 -2147483648.0f) + 2^23 = 2^39 + 2^31 + 2^23
        fadds  %r0, %r1
        ret
.size   ispu_sint_to_float, .-ispu_sint_to_float

.globl ispu_uint_to_float
.section .text.ispu_uint_to_float,"ax",@progbits
.type ispu_uint_to_float, @function
; convert an unsigned integer %r0 into a float %r0 (used r1)
.align 4
ispu_uint_to_float:
        movw   %r1, %r0         ; lower 16 bits
        uexth  %r1, %r1
        orw    %r1, 0x4b000000  ; exp 2^23
        srlw   %r0, #16
        orw    %r0, 0x53000000  ; exp 2^39  (2^23 * 2^16)
        fsubs  %r0, 0x53000080  ; sub float (549764202496) (2^23 * 2^16) + 2^23 = 2^39 + 2^23
        fadds  %r0, %r1
        ret
.size   ispu_uint_to_float, .-ispu_uint_to_float

.globl ispu_float_to_int
.section .text.ispu_float_to_int,"ax",@progbits
.type ispu_float_to_int, @function
; convert a float %r0 into a signed integer %r0 (used r1,r2,r3)
.align 4
ispu_float_to_int:
        movw   %r1, %r0
        srlw   %r1, #0x10
        movw   %r2, %r0
        srlw   %r1, #0x7
        srlw   %r2, #0x10
        srlw   %r2, #0xf
        uextb  %r1, %r1
        andw   %r0, #0x7fffff
        cmpw   %r1, #0x7f
        movb   %r3, %r2
        jpdlt  .Lf2i_0
        cmpw   %r1, #0x9e
        jpdlt  .Lf2i_1
        movw   %r0, %r2
        addw   %r0, #0x7fffffff
        ret
.Lf2i_0:
        movw   %r0, #0x0
        ret
.Lf2i_1:
        orw    %r0, #0x800000
        cmpw   %r1, #0x96
        jpdge  .Lf2i_2
        movb   %r2, #0x96
        subw   %r2, %r1
        movw   %r1, %r0
        srlw   %r1, %r2
        cmpb   %r3, #0x0
        jpdne  .Lf2i_4
.Lf2i_3:
        movw   %r0, %r1
        ret
.Lf2i_2:
        movw   %r2, %r1
        addw   %r2, #0xffffff6a
        movw   %r1, %r0
        slaw   %r1, %r2
        cmpb   %r3, #0x0
        jpdeq  .Lf2i_3
.Lf2i_4:
        movw   %r0, #0x0
        subw   %r0, %r1
        ret
.size   ispu_float_to_int, .-ispu_float_to_int

.globl __floatsisf
.set __floatsisf,ispu_sint_to_float

.globl __floatunsisf
.set __floatunsisf,ispu_uint_to_float

.globl __fixsfsi
.set __fixsfsi,ispu_float_to_int

