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

.global twiddle_table256
.data
.type twiddle_table256, @object
.size twiddle_table256, 256
.align 9
twiddle_table256:
	.long 0x3f800000
	.long 0x3f7fec43
	.long 0x3f7fb10f
	.long 0x3f7f4e6d
	.long 0x3f7ec46d
	.long 0x3f7e1324
	.long 0x3f7d3aac
	.long 0x3f7c3b28
	.long 0x3f7b14be
	.long 0x3f79c79d
	.long 0x3f7853f8
	.long 0x3f76ba07
	.long 0x3f74fa0b
	.long 0x3f731447
	.long 0x3f710908
	.long 0x3f6ed89e
	.long 0x3f6c835e
	.long 0x3f6a09a7
	.long 0x3f676bd8
	.long 0x3f64aa59
	.long 0x3f61c598
	.long 0x3f5ebe05
	.long 0x3f5b941a
	.long 0x3f584853
	.long 0x3f54db31
	.long 0x3f514d3d
	.long 0x3f4d9f02
	.long 0x3f49d112
	.long 0x3f45e403
	.long 0x3f41d870
	.long 0x3f3daef9
	.long 0x3f396842
	.long 0x3f3504f3
	.long 0x3f3085bb
	.long 0x3f2beb4a
	.long 0x3f273656
	.long 0x3f226799
	.long 0x3f1d7fd1
	.long 0x3f187fc0
	.long 0x3f13682a
	.long 0x3f0e39da
	.long 0x3f08f59b
	.long 0x3f039c3d
	.long 0x3efc5d27
	.long 0x3ef15aea
	.long 0x3ee63375
	.long 0x3edae880
	.long 0x3ecf7bca
	.long 0x3ec3ef15
	.long 0x3eb8442a
	.long 0x3eac7cd4
	.long 0x3ea09ae5
	.long 0x3e94a031
	.long 0x3e888e93
	.long 0x3e78cfcc
	.long 0x3e605c13
	.long 0x3e47c5c2
	.long 0x3e2f10a2
	.long 0x3e164083
	.long 0x3dfab273
	.long 0x3dc8bd36
	.long 0x3d96a905
	.long 0x3d48fb30
	.long 0x3cc90ab0
.global twiddle_table256_end
twiddle_table256_end:
	.long 0x00000000

