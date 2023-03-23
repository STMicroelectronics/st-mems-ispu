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

#include <stdint.h>
#include "crc8.h"

#define CRC8_POLY 0x07u

static uint8_t crc8_table[256];

void crc8_init(void)
{
	uint8_t c;

	for (uint16_t n = 0; n < 256u; n++) {
		c = (uint8_t)n;
		for (uint8_t k = 8; k > 0u; k--) {
			if ((c & 0x80u) != 0u) {
				c = (c << 1) ^ CRC8_POLY;
			} else {
				c = c << 1;
			}
		}
		crc8_table[n] = c;
	}
}

uint8_t crc8_run(const uint8_t *in, uint16_t len)
{
	uint8_t c = 0x00u;

	for (uint16_t n = 0; n < len; n++) {
		c = crc8_table[c ^ in[n]];
	}

	return c ^ 0x00u;
}

