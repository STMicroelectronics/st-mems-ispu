/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include "stai.h"

void init_network_buffers(stai_network *net, stai_ptr *input_buffers, stai_ptr *output_buffers);

#endif

