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

#include "global.S"

	.globl _start
	.globl main
	.globl fail

	.section ".init", "ax"
	define_weak_label _startup                ; weak to allow alternate _startup definition

_startup:
_interrupt_table:
	#ifdef STREDSIM
		jpia boot
	#else
		jpia trap_handler                     ; 0x00
	#endif
	jpia lo_prio_interrupt                    ; 0x04
	.long 0                                   ; 0x08
	.long 0                                   ; 0x0c
	jpia algo_00                              ; 0x10
	jpia algo_01                              ; 0x14
	jpia algo_02                              ; 0x18
	jpia algo_03                              ; 0x1c
	jpia algo_04                              ; 0x20
	jpia algo_05                              ; 0x24
	jpia algo_06                              ; 0x28
	jpia algo_07                              ; 0x2c
	jpia algo_08                              ; 0x30
	jpia algo_09                              ; 0x34
	jpia algo_10                              ; 0x38
	jpia algo_11                              ; 0x3c
	jpia algo_12                              ; 0x40
	jpia algo_13                              ; 0x44
	jpia algo_14                              ; 0x48
	jpia algo_15                              ; 0x4c
	.long 0                                   ; 0x50
	.long 0                                   ; 0x54
	.long 0                                   ; 0x58
	.long 0                                   ; 0x5c
	.long 0                                   ; 0x60
	.long 0                                   ; 0x64
	.long 0                                   ; 0x68
	.long 0                                   ; 0x6c
	.long 0                                   ; 0x70
	.long 0                                   ; 0x74
	jpia data_valid_interrupt                 ; 0x78
	.long 0                                   ; 0x7c
BOOT_PC:
	jpia boot                                 ; reset vector 0x80
	.long 0                                   ; 0x84
	.long 0                                   ; 0x88
	.long 0                                   ; 0x8c
	jpia algo_00_init                         ; 0x90
	jpia algo_01_init                         ; 0x94
	jpia algo_02_init                         ; 0x98
	jpia algo_03_init                         ; 0x9c
	jpia algo_04_init                         ; 0xa0
	jpia algo_05_init                         ; 0xa4
	jpia algo_06_init                         ; 0xa8
	jpia algo_07_init                         ; 0xac
	jpia algo_08_init                         ; 0xb0
	jpia algo_09_init                         ; 0xb4
	jpia algo_10_init                         ; 0xb8
	jpia algo_11_init                         ; 0xbc
	jpia algo_12_init                         ; 0xc0
	jpia algo_13_init                         ; 0xc4
	jpia algo_14_init                         ; 0xc8
	jpia algo_15_init                         ; 0xcc

.size _startup, .-_startup

	.section ".text", "ax"
	.globl boot
boot:
	movp %r0, _canary_addr
	stp [0x8082C], %r0
	movp %r15, #_stack
	movw %r0, #0
	movw %r1, #0
	movw %r2, #0
	movw %r3, #0
	movw %r4, #0
	movw %r5, #0
	movw %r6, #0
	movw %r7, #0
	movw %r8, #0
	movw %r9, #0
	movw %r10, #0
	movw %r11, #0
	movw %r12, #0
	movw %r13, #0
	movw %r14, #0

init_bss:
	movp %r0, BSS_VMA
	cmpp %r0, _ebss
	jpdc init_bss_end                         ; exit when no .bss section
	movw %r1, #0
init_bss_lp:
	stw [%r0]+, %r1                           ; clear from BSS_VMA
	cmpp %r0, _ebss                           ; to end
	jpdnc init_bss_lp
init_bss_end:

; if not defined elsewhere use this ones
	define_weak_label _start
_start:
	jli main
	jli exit
stpclk: set_sens_sleep
	stop_and_wait_start_pulse
	jpda stpclk

	define_weak_function trap_handler         ; 0x00
	define_weak_function algo_00              ; 0x10
	define_weak_function algo_01              ; 0x14
	define_weak_function algo_02              ; 0x18
	define_weak_function algo_03              ; 0x1c
	define_weak_function algo_04              ; 0x20
	define_weak_function algo_05              ; 0x24
	define_weak_function algo_06              ; 0x28
	define_weak_function algo_07              ; 0x2c
	define_weak_function algo_08              ; 0x30
	define_weak_function algo_09              ; 0x34
	define_weak_function algo_10              ; 0x38
	define_weak_function algo_11              ; 0x3c
	define_weak_function algo_12              ; 0x40
	define_weak_function algo_13              ; 0x44
	define_weak_function algo_14              ; 0x48
	define_weak_function algo_15              ; 0x4c
	define_weak_function overrun_handler      ; 0x50
	define_weak_function data_valid_interrupt ; 0x78
	define_weak_function algo_00_init         ; 0x90
	define_weak_function algo_01_init         ; 0x94
	define_weak_function algo_02_init         ; 0x98
	define_weak_function algo_03_init         ; 0x9c
	define_weak_function algo_04_init         ; 0xa0
	define_weak_function algo_05_init         ; 0xa4
	define_weak_function algo_06_init         ; 0xa8
	define_weak_function algo_07_init         ; 0xac
	define_weak_function algo_08_init         ; 0xb0
	define_weak_function algo_09_init         ; 0xb4
	define_weak_function algo_10_init         ; 0xb8
	define_weak_function algo_11_init         ; 0xbc
	define_weak_function algo_12_init         ; 0xc0
	define_weak_function algo_13_init         ; 0xc4
	define_weak_function algo_14_init         ; 0xc8
	define_weak_function algo_15_init         ; 0xcc
	rfe
	define_weak_function lo_prio_interrupt    ; 0x04
	subw %r15, #4
	stw [%r15], %r0                           ; save r0
	ldw %r0, [ctrl_reg_ext]                   ; clear interrupt
	ldw %r0, [%r15]+                          ; restore r0
	rfe
.size _start, .-_start

	define_weak_function main
.globl hexfloat
hexfloat:                                     ; convert a float in %r0 into its hex form (leave it as it is)
.globl __main
__main:
	ret
.size __main, .-__main

