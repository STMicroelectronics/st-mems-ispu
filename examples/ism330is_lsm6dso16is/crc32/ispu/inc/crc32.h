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

#ifndef CRC32_H
#define CRC32_H

#include <stdint.h>

void crc32_init(void);
uint32_t crc32_run(const uint8_t *in, uint16_t len);

#endif

