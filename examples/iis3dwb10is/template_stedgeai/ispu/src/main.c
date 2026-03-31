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

#include "peripherals.h"
#include "reg_map.h"
#include "ispu_float.h"

#include <stdint.h>
#include <stdbool.h>

#include "stai.h"
#include "network.h"
#include "network_utils.h"

static __attribute__((aligned(8))) stai_network net[STAI_NETWORK_CONTEXT_SIZE];

static stai_ptr input_buffers[STAI_NETWORK_IN_NUM];
static stai_ptr output_buffers[STAI_NETWORK_OUT_NUM];

static volatile uint16_t int_status;

void __attribute__ ((signal)) data_valid_interrupt(void)
{
	#warning "Read the sensor data."
}

void __attribute__ ((signal)) lo_prio_interrupt(void)
{
	// TODO: process the interrupt based on the trigger source(s) indicated by ISPU_LOPRIO_INT
}

void __attribute__ ((signal)) algo_00_init(void)
{
	(void)stai_runtime_init();
	(void)stai_network_init(net); // initialize the network context

	init_network_buffers(net, input_buffers, output_buffers);
}

void __attribute__ ((signal)) algo_00(void)
{
	#warning "Fill the input data buffers contained in input_buffers."
	#warning "Each buffer in input_buffers must be cast to the appropriate type before accessing it."

	#warning "The network inference must be run when the input data buffers are ready."
	stai_return_code res = stai_network_run(net, STAI_MODE_SYNC);
	if (res >= STAI_ERROR_GENERIC) {
		#warning "Handle inference error as deemed appropriate."
	}

	#warning "Get the inference results from the output data buffers contained in output_buffers."
	#warning "Each buffer in output_buffers must be cast to the appropriate type before accessing it."

	// interrupt generation
	int_status = int_status | 0x1u;
}

int main(void)
{
	// set boot end flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x40u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable high-priority (data valid) interrupt, low-priority interrupt, and algorithms interrupts
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x0D;

	while (true) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0u;
		cast_uint16_t(ISPU_INT_STATUS) = 0u;
		cast_uint8_t(ISPU_INT_PIN) = 0u;

		// trigger enabled algorithms execution
		cast_uint32_t(ISPU_CALL_EN) = cast_uint16_t(ISPU_ALGO) << 4;

		// wait for all algorithms execution
		while (cast_uint32_t(ISPU_CALL_EN & 0xFFFF0) != 0u)
			;

		// get interrupt flags
		uint8_t int_pin = 0u;
		int_pin |= ((int_status & cast_uint16_t(ISPU_INT1_CTRL)) > 0u) ? 0x01u : 0x00u;
		int_pin |= ((int_status & cast_uint16_t(ISPU_INT2_CTRL)) > 0u) ? 0x02u : 0x00u;

		// set status registers and generate interrupts
		cast_uint16_t(ISPU_INT_STATUS) = int_status;
		cast_uint8_t(ISPU_INT_PIN) = int_pin;
	}
}

