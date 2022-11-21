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

#include "reg_map.h"

#define	JOIN(a, b)	a ## b

.macro define_weak_label label
.weak	\label
.type	\label,	@function
.endm

.macro define_weak_function func
define_weak_label \func
\func:
.endm

.macro stop_and_wait_start_pulse
	ldb	%r0, [STOP_CLOCK]
.endm
