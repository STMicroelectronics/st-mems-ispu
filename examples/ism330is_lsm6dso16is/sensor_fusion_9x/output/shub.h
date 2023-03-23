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

#ifndef SHUB_H
#define SHUB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef MEMS_UCF_EXT_SHARED_TYPES
#define MEMS_UCF_EXT_SHARED_TYPES

/*
 * UCF extended format supports the following commands:
 * - MEMS_UCF_OP_READ: read the register at the location specified by the
 *   "address" field ("data" field is ignored
 * - MEMS_UCF_OP_WRITE: write the value specified by the "data" field at the
 *   location specified by the "address" field
 * - MEMS_UCF_OP_DELAY: wait the number of milliseconds specified by the "data"
 *   field ("address" field is ignored)
 * - MEMS_UCF_OP_POLL_SET: poll the register at the location specified by the
 *   "address" field until all the bits identified by the mask specified by the
 *   "data" field are set to 1
 * - MEMS_UCF_OP_POLL_RESET: poll the register at the location specified by the
 *   "address" field until all the bits identified by the mask specified by the
 *   "data" field are reset to 0
 */

#define MEMS_UCF_OP_READ       0
#define MEMS_UCF_OP_WRITE      1
#define MEMS_UCF_OP_DELAY      2
#define MEMS_UCF_OP_POLL_SET   3
#define MEMS_UCF_OP_POLL_RESET 4

typedef struct {
	uint8_t op;
	uint8_t address;
	uint8_t data;
} ucf_line_ext_t;

#endif /* MEMS_UCF_EXT_SHARED_TYPES */

static const ucf_line_ext_t shub_conf[] = {
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x10, .data = 0x40 },
	// write [device 0x3C], 0x60 = 0x8C
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x40 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x15, .data = 0x3C },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x16, .data = 0x60 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x17, .data = 0x00 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x21, .data = 0x8C },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x14, .data = 0x44 },
	{ .op = MEMS_UCF_OP_DELAY, .address = 0, .data = 20 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x14, .data = 0x00 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x00 },
	// write [device 0x3C], 0x61 = 0x02
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x40 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x15, .data = 0x3C },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x16, .data = 0x61 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x17, .data = 0x00 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x21, .data = 0x02 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x14, .data = 0x44 },
	{ .op = MEMS_UCF_OP_DELAY, .address = 0, .data = 20 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x14, .data = 0x00 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x00 },
	// write [device 0x3C], 0x62 = 0x10
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x40 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x15, .data = 0x3C },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x16, .data = 0x62 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x17, .data = 0x00 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x21, .data = 0x10 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x14, .data = 0x44 },
	{ .op = MEMS_UCF_OP_DELAY, .address = 0, .data = 20 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x14, .data = 0x00 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x00 },
	// read [device 0x3C], from 0x68 for 6 bytes
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x40 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x15, .data = 0x3D },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x16, .data = 0x68 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x17, .data = 0x06 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x00 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x40 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x14, .data = 0x44 },
	{ .op = MEMS_UCF_OP_WRITE, .address = 0x01, .data = 0x00 },
};

#ifdef __cplusplus
}
#endif

#endif

