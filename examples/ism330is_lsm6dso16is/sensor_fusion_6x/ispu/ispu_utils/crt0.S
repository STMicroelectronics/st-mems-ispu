/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#include "global.S"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.globl _start
	.globl main
	.globl fail

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.section ".init", "ax"
	define_weak_label _startup	; weak to allow alternate _startup definition

/********************************************************************************
  there must be N_ALGO interrupt handlers
  followed by an other set of N_ALGO interrupt handlers (the init ones)
********************************************************************************/
_startup:
_interrupt_table:
	jpia boot			; the first vector is the reset vector	0x00
	jpia algo_00			; interrupt_handler1				0x04
	jpia algo_01			; interrupt_handler2				0x08
	jpia algo_02			; interrupt_handler3				0x0c
	jpia algo_03			; interrupt_handler4				0x10
	jpia algo_04			; interrupt_handler5				0x14
	jpia algo_05			; interrupt_handler6				0x18
	jpia algo_06			; interrupt_handler7				0x1c
	jpia algo_07			; interrupt_handler8				0x20
	jpia algo_08			; interrupt_handler9				0x24
	jpia algo_09			; interrupt_handler10				0x28
	jpia algo_10			; interrupt_handler11				0x2c
	jpia algo_11			; interrupt_handler12				0x30
	jpia algo_12			; interrupt_handler13				0x34
	jpia algo_13			; interrupt_handler14				0x38
	jpia algo_14			; interrupt_handler15				0x3c
	jpia algo_15			; interrupt_handler16				0x40
	jpia algo_16			; interrupt_handler17				0x44
	jpia algo_17			; interrupt_handler18				0x48
	jpia algo_18			; interrupt_handler19				0x4c
	jpia algo_19			; interrupt_handler20				0x50
	jpia algo_20			; interrupt_handler21				0x54
	jpia algo_21			; interrupt_handler22				0x58
	jpia algo_22			; interrupt_handler23				0x5c
	jpia algo_23			; interrupt_handler24				0x60
	jpia algo_24			; interrupt_handler25				0x64
	jpia algo_25			; interrupt_handler26				0x68
	jpia algo_26			; interrupt_handler27				0x6c
	jpia algo_27			; interrupt_handler28				0x70
	jpia algo_28			; interrupt_handler29				0x74
	jpia algo_29			; interrupt_handler30				0x78
	jpia reserved			; interrupt_handler31				0x7c
	jpia algo_00_init		; first occurrence of algo_00			0x80
	jpia algo_01_init		; first occurrence of algo_01			0x84
	jpia algo_02_init		; first occurrence of algo_02			0x88
	jpia algo_03_init		; first occurrence of algo_03			0x8c
	jpia algo_04_init		; first occurrence of algo_04			0x90
	jpia algo_05_init		; first occurrence of algo_05			0x94
	jpia algo_06_init		; first occurrence of algo_06			0x98
	jpia algo_07_init		; first occurrence of algo_07			0x9c
	jpia algo_08_init		; first occurrence of algo_08			0xa0
	jpia algo_09_init		; first occurrence of algo_09			0xa4
	jpia algo_10_init		; first occurrence of algo_10			0xa8
	jpia algo_11_init		; first occurrence of algo_11			0xac
	jpia algo_12_init		; first occurrence of algo_12			0xb0
	jpia algo_13_init		; first occurrence of algo_13			0xb4
	jpia algo_14_init		; first occurrence of algo_14			0xb8
	jpia algo_15_init		; first occurrence of algo_15			0xbc
	jpia algo_16_init		; first occurrence of algo_16			0xc0
	jpia algo_17_init		; first occurrence of algo_17			0xc4
	jpia algo_18_init		; first occurrence of algo_18			0xc8
	jpia algo_19_init		; first occurrence of algo_19			0xcc
	jpia algo_20_init		; first occurrence of algo_20			0xd0
	jpia algo_21_init		; first occurrence of algo_21			0xd4
	jpia algo_22_init		; first occurrence of algo_22			0xd8
	jpia algo_23_init		; first occurrence of algo_23			0xdc
	jpia algo_24_init		; first occurrence of algo_24			0xe0
	jpia algo_25_init		; first occurrence of algo_25			0xe4
	jpia algo_26_init		; first occurrence of algo_26			0xe8
	jpia algo_27_init		; first occurrence of algo_27			0xec
	jpia algo_28_init		; first occurrence of algo_28			0xf0
	jpia algo_29_init		; first occurrence of algo_29			0xf4
.size _startup, .-_startup

	.section ".text", "ax"
	.globl	boot
boot:	movp	%r15, #_stack
	movw	%r0, #0
	movw	%r1, #0
	movw	%r2, #0
	movw	%r3, #0
	movw	%r4, #0
	movw	%r5, #0
	movw	%r6, #0
	movw	%r7, #0
	movw	%r8, #0
	movw	%r9, #0
	movw	%r10, #0
	movw	%r11, #0
	movw	%r12, #0
	movw	%r13, #0
	movw	%r14, #0

init_bss:
	movp	%r0, BSS_VMA
	cmpp	%r0, _ebss
	jpdc	init_bss_end		; exit when no .bss section
	movw	%r1, #0
init_bss_lp:
	stw	[%r0]+, %r1		; clear from BSS_VMA
	cmpp	%r0, _ebss		; to end
	jpdnc	init_bss_lp
init_bss_end:

init_data:
	movp	%r1, DATA_VMA
	cmpp	%r1, DATA_LMA
	jpdeq	init_data_end
	cmpp	%r1, _edata
	jpdc	init_data_end		; exit when no initialized data
	movp	%r2, DATA_LMA
init_data_lp:
	ldw	%r0, [%r2]		; copy from address DATA_LMA
	addw	%r2, #4
	stw	[%r1]+, %r0		; to DATA_VMA
	cmpp	%r1, _edata		; until end
	jpdnc	init_data_lp
init_data_end:

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; if not defined elsewhere use this ones
	define_weak_label _start
_start:
	jli	main
	jli exit
stpclk:	stop_and_wait_start_pulse
	jpda	stpclk

	define_weak_function	algo_00			; interrupt_handler1				0x04
	define_weak_function	algo_01			; interrupt_handler2				0x08
	define_weak_function	algo_02			; interrupt_handler3				0x0c
	define_weak_function	algo_03			; interrupt_handler4				0x10
	define_weak_function	algo_04			; interrupt_handler5				0x14
	define_weak_function	algo_05			; interrupt_handler6				0x18
	define_weak_function	algo_06			; interrupt_handler7				0x1c
	define_weak_function	algo_07			; interrupt_handler8				0x20
	define_weak_function	algo_08			; interrupt_handler9				0x24
	define_weak_function	algo_09			; interrupt_handler10				0x28
	define_weak_function	algo_10			; interrupt_handler11				0x2c
	define_weak_function	algo_11			; interrupt_handler12				0x30
	define_weak_function	algo_12			; interrupt_handler13				0x34
	define_weak_function	algo_13			; interrupt_handler14				0x38
	define_weak_function	algo_14			; interrupt_handler15				0x3c
	define_weak_function	algo_15			; interrupt_handler16				0x40
	define_weak_function	algo_16			; interrupt_handler17				0x44
	define_weak_function	algo_17			; interrupt_handler18				0x48
	define_weak_function	algo_18			; interrupt_handler19				0x4c
	define_weak_function	algo_19			; interrupt_handler20				0x50
	define_weak_function	algo_20			; interrupt_handler21				0x54
	define_weak_function	algo_21			; interrupt_handler22				0x58
	define_weak_function	algo_22			; interrupt_handler23				0x5c
	define_weak_function	algo_23			; interrupt_handler24				0x60
	define_weak_function	algo_24			; interrupt_handler25				0x64
	define_weak_function	algo_25			; interrupt_handler26				0x68
	define_weak_function	algo_26			; interrupt_handler27				0x6c
	define_weak_function	algo_27			; interrupt_handler28				0x70
	define_weak_function	algo_28			; interrupt_handler29				0x74
	define_weak_function	algo_29			; interrupt_handler30				0x78
	define_weak_function	reserved		; interrupt_handler31				0x7c
	define_weak_function	algo_00_init		; first occurrence of interrupt_handler1	0x48
	define_weak_function	algo_01_init		; first occurrence of interrupt_handler2	0x4c
	define_weak_function	algo_02_init		; first occurrence of interrupt_handler3	0x50
	define_weak_function	algo_03_init		; first occurrence of interrupt_handler4	0x54
	define_weak_function	algo_04_init		; first occurrence of interrupt_handler5	0x58
	define_weak_function	algo_05_init		; first occurrence of interrupt_handler6	0x5c
	define_weak_function	algo_06_init		; first occurrence of interrupt_handler7	0x60
	define_weak_function	algo_07_init		; first occurrence of interrupt_handler8	0x64
	define_weak_function	algo_08_init		; first occurrence of interrupt_handler9	0x68
	define_weak_function	algo_09_init		; first occurrence of interrupt_handler10	0x6c
	define_weak_function	algo_10_init		; first occurrence of interrupt_handler11	0x70
	define_weak_function	algo_11_init		; first occurrence of interrupt_handler12	0x74
	define_weak_function	algo_12_init		; first occurrence of interrupt_handler13	0x78
	define_weak_function	algo_13_init		; first occurrence of interrupt_handler14	0x8c
	define_weak_function	algo_14_init		; first occurrence of interrupt_handler15	0x80
	define_weak_function	algo_15_init		; first occurrence of interrupt_handler16	0x84
	define_weak_function	algo_16_init		; first occurrence of interrupt_handler17	0x88
	define_weak_function	algo_17_init		; first occurrence of interrupt_handler18	0x8c
	define_weak_function	algo_18_init		; first occurrence of interrupt_handler19	0x90
	define_weak_function	algo_19_init		; first occurrence of interrupt_handler20	0x94
	define_weak_function	algo_20_init		; first occurrence of interrupt_handler21	0x98
	define_weak_function	algo_21_init		; first occurrence of interrupt_handler22	0x9c
	define_weak_function	algo_22_init		; first occurrence of interrupt_handler23	0xa0
	define_weak_function	algo_23_init		; first occurrence of interrupt_handler24	0xa4
	define_weak_function	algo_24_init		; first occurrence of interrupt_handler25	0xa8
	define_weak_function	algo_25_init		; first occurrence of interrupt_handler26	0xac
	define_weak_function	algo_26_init		; first occurrence of interrupt_handler27	0xb0
	define_weak_function	algo_27_init		; first occurrence of interrupt_handler28	0xb4
	define_weak_function	algo_28_init		; first occurrence of interrupt_handler29	0xb8
	define_weak_function	algo_29_init		; first occurrence of interrupt_handler30	0xbc

	rfe
.size _start, .-_start

; if not defined elsewhere use this ones
	define_weak_function	main
.globl hexfloat
hexfloat:	; convert a float number in %r0 into its hex equivalent (leave as it is)
.globl	__main
__main:
	ret
.size __main, .-__main

