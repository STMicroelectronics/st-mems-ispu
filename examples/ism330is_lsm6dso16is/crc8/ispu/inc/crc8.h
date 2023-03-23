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

#ifndef CRC8_H
#define CRC8_H

#include <stdint.h>

void crc8_init(void);
uint8_t crc8_run(const uint8_t *in, uint16_t len);

#endif

