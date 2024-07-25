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

#include <stdint.h>

#include "network_utils.h"

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

void init_network_buffers(stai_network *net, stai_ptr *input_buffers, stai_ptr *output_buffers)
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

	// set the pointers to the activation, input, output and state buffers if not already allocated
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

