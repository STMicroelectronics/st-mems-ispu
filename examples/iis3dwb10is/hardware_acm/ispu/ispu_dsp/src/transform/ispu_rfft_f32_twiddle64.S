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

.global twiddle_table64
.data
.type twiddle_table64, @object
.size twiddle_table64, 64
.align 8
twiddle_table64:
	.long 0x3f800000
	.long 0x3f7ec46d
	.long 0x3f7b14be
	.long 0x3f74fa0b
	.long 0x3f6c835e
	.long 0x3f61c598
	.long 0x3f54db31
	.long 0x3f45e403
	.long 0x3f3504f3
	.long 0x3f226799
	.long 0x3f0e39da
	.long 0x3ef15aea
	.long 0x3ec3ef15
	.long 0x3e94a031
	.long 0x3e47c5c2
	.long 0x3dc8bd36
.global twiddle_table64_end
twiddle_table64_end:
	.long 0x00000000

