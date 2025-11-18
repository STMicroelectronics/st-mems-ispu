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
#include "crc32.h"

#define CRC32_POLY 0x04C11DB7u

static uint32_t crc32_table[256];

void crc32_init(void)
{
	uint32_t c;

	for (uint32_t n = 0; n < 256u; n++) {
		c = n << 24;
		for (uint8_t k = 8; k > 0u; k--) {
			if ((c & 0x80000000u) != 0u) {
				c = (c << 1) ^ CRC32_POLY;
			} else {
				c = c << 1;
			}
		}
		crc32_table[n] = c;
	}
}

uint32_t crc32_run(const uint8_t *in, uint16_t len)
{
	uint32_t c = 0xFFFFFFFFu;

	for (uint16_t n = 0; n < len; n++) {
		c = crc32_table[((c >> 24) ^ in[n]) & 0xFFu] ^ (c << 8);
	}

	return c ^ 0xFFFFFFFFu;
}

