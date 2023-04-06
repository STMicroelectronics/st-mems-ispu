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

#include "peripherals.h"
#include "reg_map.h"

#include <stdint.h>
#include <stdbool.h>

#include "stai.h"
#include "network.h"

#define ANY_BUF_ALLOC ( \
		!(STAI_NETWORK_FLAGS & STAI_FLAG_INPUTS) || \
		!(STAI_NETWORK_FLAGS & STAI_FLAG_OUTPUTS) || \
		!(STAI_NETWORK_FLAGS & STAI_FLAG_ACTIVATIONS) || \
		(!(STAI_NETWORK_FLAGS & STAI_FLAG_STATES) && (STAI_NETWORK_STATES_NUM != 0)) \
	)

#define ANY_BUF_GET ( \
		(STAI_NETWORK_FLAGS & STAI_FLAG_INPUTS) || \
		(STAI_NETWORK_FLAGS & STAI_FLAG_OUTPUTS) \
	)

void __attribute__ ((signal)) algo_00_init(void);
void __attribute__ ((signal)) algo_00(void);

static void init_network(void);

static __attribute__((aligned(8))) stai_network net[STAI_NETWORK_CONTEXT_SIZE];

static stai_ptr input_buffers[STAI_NETWORK_IN_NUM];
static stai_ptr output_buffers[STAI_NETWORK_OUT_NUM];

static volatile uint32_t int_status;

static void init_network(void)
{
#if !(STAI_NETWORK_FLAGS & STAI_FLAG_INPUTS)
	static uint8_t inputs_mem[STAI_NETWORK_IN_SIZE_BYTES];
	const uint32_t input_sizes[STAI_NETWORK_IN_NUM] = STAI_NETWORK_IN_SIZES_BYTES;
	const uint8_t input_aligns[STAI_NETWORK_IN_NUM] = STAI_NETWORK_IN_ALIGNMENTS;
#endif
#if !(STAI_NETWORK_FLAGS & STAI_FLAG_OUTPUTS)
	static uint8_t outputs_mem[STAI_NETWORK_OUT_SIZE_BYTES];
	const uint32_t output_sizes[STAI_NETWORK_OUT_NUM] = STAI_NETWORK_OUT_SIZES_BYTES;
	const uint8_t output_aligns[STAI_NETWORK_OUT_NUM] = STAI_NETWORK_OUT_ALIGNMENTS;
#endif
#if !(STAI_NETWORK_FLAGS & STAI_FLAG_ACTIVATIONS)
	static uint8_t activations_mem[STAI_NETWORK_ACTIVATIONS_SIZE_BYTES];
	const uint32_t activation_sizes[STAI_NETWORK_ACTIVATIONS_NUM] = STAI_NETWORK_ACTIVATIONS_SIZES;
	const uint8_t activation_aligns[STAI_NETWORK_ACTIVATIONS_NUM] = STAI_NETWORK_ACTIVATIONS_ALIGNMENTS;
	stai_ptr activation_buffers[STAI_NETWORK_ACTIVATIONS_NUM];
#endif
#if !(STAI_NETWORK_FLAGS & STAI_FLAG_STATES) && (STAI_NETWORK_STATES_NUM != 0)
	static uint8_t states_mem[STAI_NETWORK_STATES_SIZE_BYTES];
	const uint32_t state_sizes[STAI_NETWORK_STATES_NUM] = STAI_NETWORK_STATES_SIZES;
	const uint8_t state_aligns[STAI_NETWORK_STATES_NUM] = STAI_NETWORK_STATES_ALIGNMENTS;
	stai_ptr state_buffers[STAI_NETWORK_STATES_NUM];
#endif

#if ANY_BUF_ALLOC
	uint32_t curr;
#endif
#if ANY_BUF_GET
	stai_size tmp;
#endif

	(void)stai_runtime_init();
	(void)stai_network_init(net); // initialize the network context

	// set the pointers to the input, output and activation buffers if not already allocated
	// if already allocated in the activation buffer, retrieve pointers to the input and output buffers

#if !(STAI_NETWORK_FLAGS & STAI_FLAG_ACTIVATIONS)
	curr = (uint32_t)activations_mem;
	for (int8_t i = 0; i < STAI_NETWORK_ACTIVATIONS_NUM; i++) {
		uint32_t alignment = curr % activation_aligns[i];
		if (alignment != 0u) {
			curr += activation_aligns[i] - alignment;
		}

		activation_buffers[i] = (stai_ptr)curr;

		curr += activation_sizes[i];
	}
	// set activation buffer first in case inputs and outputs are allocated inside
	(void)stai_network_set_activations(net, activation_buffers, STAI_NETWORK_ACTIVATIONS_NUM);
#endif

#if (STAI_NETWORK_FLAGS & STAI_FLAG_INPUTS)
	stai_network_get_inputs(net, input_buffers, &tmp);
#else
	curr = (uint32_t)inputs_mem;
	for (int8_t i = 0; i < STAI_NETWORK_IN_NUM; i++) {
		uint32_t alignment = curr % input_aligns[i];
		if (alignment != 0u) {
			curr += input_aligns[i] - alignment;
		}

		input_buffers[i] = (stai_ptr)curr;

		curr += input_sizes[i];
	}
	(void)stai_network_set_inputs(net, input_buffers, STAI_NETWORK_IN_NUM);
#endif

#if (STAI_NETWORK_FLAGS & STAI_FLAG_OUTPUTS)
	stai_network_get_outputs(net, output_buffers, &tmp);
#else
	curr = (uint32_t)outputs_mem;
	for (int8_t i = 0; i < STAI_NETWORK_OUT_NUM; i++) {
		uint32_t alignment = curr % output_aligns[i];
		if (alignment != 0u) {
			curr += output_aligns[i] - alignment;
		}

		output_buffers[i] = (stai_ptr)curr;

		curr += output_sizes[i];
	}
	(void)stai_network_set_outputs(net, output_buffers, STAI_NETWORK_OUT_NUM);
#endif

#if !(STAI_NETWORK_FLAGS & STAI_FLAG_STATES) && (STAI_NETWORK_STATES_NUM != 0)
	curr = (uint32_t)states_mem;
	for (int8_t i = 0; i < STAI_NETWORK_STATES_NUM; i++) {
		uint32_t alignment = curr % state_aligns[i];
		if (alignment != 0u) {
			curr += state_aligns[i] - alignment;
		}

		state_buffers[i] = (stai_ptr)curr;

		curr += state_sizes[i];
	}
	(void)stai_network_set_states(net, state_buffers, STAI_NETWORK_STATES_NUM);
#endif
}

void __attribute__ ((signal)) algo_00_init(void)
{
	init_network();
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
	// set boot done flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x04u;
	cast_uint8_t(ISPU_STATUS) = status;

	// enable algorithms interrupt request generation
	cast_uint8_t(ISPU_GLB_CALL_EN) = 0x01u;

	while (true) {
		stop_and_wait_start_pulse;

		// reset status registers and interrupts
		int_status = 0u;
		cast_uint32_t(ISPU_INT_STATUS) = 0u;
		cast_uint8_t(ISPU_INT_PIN) = 0u;

		// get all the algorithms to run in this time slot
		cast_uint32_t(ISPU_CALL_EN) = cast_uint32_t(ISPU_ALGO) << 1;

		// wait for all algorithms execution
		while (cast_uint32_t(ISPU_CALL_EN) != 0u) {
		}

		// get interrupt flags
		uint8_t int_pin = 0u;
		int_pin |= ((int_status & cast_uint32_t(ISPU_INT1_CTRL)) > 0u) ? 0x01u : 0x00u;
		int_pin |= ((int_status & cast_uint32_t(ISPU_INT2_CTRL)) > 0u) ? 0x02u : 0x00u;

		// set status registers and generate interrupts
		cast_uint32_t(ISPU_INT_STATUS) = int_status;
		cast_uint8_t(ISPU_INT_PIN) = int_pin;
	}
}

