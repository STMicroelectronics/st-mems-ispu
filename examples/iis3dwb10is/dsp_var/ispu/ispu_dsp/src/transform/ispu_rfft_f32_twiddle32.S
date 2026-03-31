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

.global twiddle_table32
.data
.type twiddle_table32, @object
.size twiddle_table32, 32
.align 8
twiddle_table32:
	.long 0x3f800000
	.long 0x3f7b14be
	.long 0x3f6c835e
	.long 0x3f54db31
	.long 0x3f3504f3
	.long 0x3f0e39da
	.long 0x3ec3ef15
	.long 0x3e47c5c2
.global twiddle_table32_end
twiddle_table32_end:
 .long 0x00000000

