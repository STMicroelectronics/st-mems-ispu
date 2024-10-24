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
#include <string.h>

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

static __attribute__((aligned(8))) stai_network net[STAI_NETWORK_CONTEXT_SIZE];

static stai_ptr input_buffers[STAI_NETWORK_IN_NUM];
static stai_ptr output_buffers[STAI_NETWORK_OUT_NUM];
static const uint32_t output_sizes[STAI_NETWORK_OUT_NUM] = STAI_NETWORK_OUT_SIZES_BYTES;
static const uint32_t activation_sizes[STAI_NETWORK_ACTIVATIONS_NUM] = STAI_NETWORK_ACTIVATIONS_SIZES;

// save adresses for host
static void set_addresses(void)
{
	static const char net_name[] = STAI_NETWORK_MODEL_NAME;
	static const uint32_t weight_sizes[STAI_NETWORK_WEIGHTS_NUM] = STAI_NETWORK_WEIGHTS_SIZES;

	// set after first 4 bytes used dinamically
	uint32_t out_addr = ISPU_DOUT_00 + 4;

	cast_uint32_t(out_addr) = (uint32_t)net_name;
	out_addr += 4u;

	cast_uint8_t(out_addr) = STAI_NETWORK_IN_NUM;
	out_addr += 1u;
	cast_uint32_t(out_addr) = (uint32_t)input_buffers;
	out_addr += 4u;

	cast_uint8_t(out_addr) = STAI_NETWORK_OUT_NUM;
	out_addr += 1u;
	cast_uint32_t(out_addr) = (uint32_t)output_buffers;
	out_addr += 4u;
	cast_uint32_t(out_addr) = (uint32_t)output_sizes;
	out_addr += 4u;

	stai_network_info info;
	(void)memset(&info, 0, sizeof(info));
	(void)stai_network_get_info(net, &info);
	cast_uint64_t(out_addr) = (uint64_t)info.n_macc;
	out_addr += 8u;

	cast_uint8_t(out_addr) = STAI_NETWORK_ACTIVATIONS_NUM;
	out_addr += 1u;
	cast_uint32_t(out_addr) = (uint32_t)activation_sizes;
	out_addr += 4u;

	cast_uint8_t(out_addr) = STAI_NETWORK_WEIGHTS_NUM;
	out_addr += 1u;
	cast_uint32_t(out_addr) = (uint32_t)weight_sizes;
	out_addr += 4u;
}

static void init_network(void)
{
#if !(STAI_NETWORK_FLAGS & STAI_FLAG_INPUTS)
	static uint8_t inputs_mem[STAI_NETWORK_IN_SIZE_BYTES];
	const uint32_t input_sizes[STAI_NETWORK_IN_NUM] = STAI_NETWORK_IN_SIZES_BYTES;
	const uint8_t input_aligns[STAI_NETWORK_IN_NUM] = STAI_NETWORK_IN_ALIGNMENTS;
#endif
#if !(STAI_NETWORK_FLAGS & STAI_FLAG_OUTPUTS)
	static uint8_t outputs_mem[STAI_NETWORK_OUT_SIZE_BYTES];
	const uint8_t output_aligns[STAI_NETWORK_OUT_NUM] = STAI_NETWORK_OUT_ALIGNMENTS;
#endif
#if !(STAI_NETWORK_FLAGS & STAI_FLAG_ACTIVATIONS)
	static uint8_t activations_mem[STAI_NETWORK_ACTIVATIONS_SIZE_BYTES];
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

static void set_version(void)
{
	const uint8_t version[3] = { 1, 1, 0 };

	cast_uint8_t(ISPU_DOUT_28 + 1) = version[0];
	cast_uint8_t(ISPU_DOUT_28 + 2) = version[1];
	cast_uint8_t(ISPU_DOUT_28 + 3) = version[2];
}

static void run_network(void)
{
	cast_uint8_t(ISPU_INT_PIN) = 0u;
	stai_return_code res = stai_network_run(net, STAI_MODE_SYNC);
	cast_uint8_t(ISPU_INT_PIN) = 2u;

	cast_uint32_t(ISPU_DOUT_00) = (uint32_t)res;
}

static void signal_and_wait(void)
{
	cast_uint8_t(ISPU_S2IF_FLAG) = 0x01u;
	while (cast_uint8_t(ISPU_S2IF_FLAG) == 0x01u) {
	}
}

static void send_tensor_info(uint16_t num, const stai_tensor *tensors, const int32_t *indexes)
{
	cast_uint16_t(ISPU_DOUT_00) = num;
	signal_and_wait();

	for (uint16_t i = 0; i < num; i++) {
		int32_t idx = indexes ? indexes[i] : i;

		cast_uint32_t(ISPU_DOUT_00) = (uint32_t)tensors[idx].format;
		signal_and_wait();

		cast_uint32_t(ISPU_DOUT_00) = tensors[idx].shape.size;
		signal_and_wait();
		for (uint8_t j = 0; j < tensors[idx].shape.size; j++) {
			cast_sint32_t(ISPU_DOUT_00) = tensors[idx].shape.data[j];
			signal_and_wait();
		}

		cast_uint32_t(ISPU_DOUT_00) = tensors[idx].scale.size;
		signal_and_wait();
		for (uint8_t j = 0; j < tensors[idx].scale.size; j++) {
			cast_float(ISPU_DOUT_00) = tensors[idx].scale.data[j];
			signal_and_wait();
		}

		cast_uint32_t(ISPU_DOUT_00) = tensors[idx].zeropoint.size;
		signal_and_wait();
		for (uint8_t j = 0; j < tensors[idx].zeropoint.size; j++) {
			cast_sint16_t(ISPU_DOUT_00) = tensors[idx].zeropoint.data[j];
			signal_and_wait();
		}

		if (tensors[idx].name == NULL) {
			cast_uint32_t(ISPU_DOUT_00) = 0;
			signal_and_wait();
		} else {
			cast_uint32_t(ISPU_DOUT_00) = strlen(tensors[idx].name);
			signal_and_wait();
			for (uint32_t j = 0; j < strlen(tensors[idx].name); j += 4) {
				strncpy((char *)ISPU_DOUT_00, &tensors[idx].name[j], 4);
				signal_and_wait();
			}
		}
	}
}

static void send_io_info(uint8_t req)
{
	stai_network_info info;
	(void)memset(&info, 0, sizeof(info));
	(void)stai_network_get_info(net, &info);

	if (req == 0u) {
		send_tensor_info(info.n_inputs, info.inputs, NULL);
	}
	if (req == 1u) {
		send_tensor_info(info.n_outputs, info.outputs, NULL);
	}
}

extern const stai_network_details g_network_details;

static void send_nodes(void)
{
	cast_uint32_t(ISPU_DOUT_00) = g_network_details.n_nodes;
	signal_and_wait();

	for (uint32_t i = 0; i < g_network_details.n_nodes; i++) {
		cast_sint32_t(ISPU_DOUT_00) = g_network_details.nodes[i].id;
		signal_and_wait();
		cast_sint16_t(ISPU_DOUT_00) = g_network_details.nodes[i].type;
		signal_and_wait();

		send_tensor_info(g_network_details.nodes[i].input_tensors.size, g_network_details.tensors, g_network_details.nodes[i].input_tensors.data);
		send_tensor_info(g_network_details.nodes[i].output_tensors.size, g_network_details.tensors, g_network_details.nodes[i].output_tensors.data);
	}
}

static void send_versions(void)
{
	stai_network_info info;
	(void)memset(&info, 0, sizeof(info));
	(void)stai_network_get_info(net, &info);

	cast_uint8_t(ISPU_DOUT_00) = info.runtime_version.major;
	signal_and_wait();
	cast_uint8_t(ISPU_DOUT_00) = info.runtime_version.minor;
	signal_and_wait();
	cast_uint8_t(ISPU_DOUT_00) = info.runtime_version.micro;
	signal_and_wait();

	cast_uint8_t(ISPU_DOUT_00) = info.tool_version.major;
	signal_and_wait();
	cast_uint8_t(ISPU_DOUT_00) = info.tool_version.minor;
	signal_and_wait();
	cast_uint8_t(ISPU_DOUT_00) = info.tool_version.micro;
	signal_and_wait();
}

int main(void)
{
	cast_uint8_t(ISPU_INT_PIN) = 2u;

	init_network();
	set_addresses();
	set_version();

	// set boot done flag
	uint8_t status = cast_uint8_t(ISPU_STATUS);
	status = status | 0x04u;
	cast_uint8_t(ISPU_STATUS) = status;

	while (true) {
		while (cast_uint8_t(ISPU_IF2S_FLAG) == 0u) { // wait for commands
			stop_and_wait_start_pulse;
		}

		cast_uint8_t(ISPU_INT_PIN) = cast_uint8_t(ISPU_INT_PIN) & 2u; // reset INT1

		uint8_t run = cast_uint8_t(ISPU_IF2S_FLAG) & 0x01u;
		uint8_t info_in = cast_uint8_t(ISPU_IF2S_FLAG) & 0x02u;
		uint8_t info_out = cast_uint8_t(ISPU_IF2S_FLAG) & 0x04u;
		uint8_t versions = cast_uint8_t(ISPU_IF2S_FLAG) & 0x08u;
		uint8_t nodes = cast_uint8_t(ISPU_IF2S_FLAG) & 0x10u;

		if (run != 0u) {
			run_network();
		}

		if (info_in != 0u) {
			send_io_info(0);
		}

		if (info_out != 0u) {
			send_io_info(1);
		}

		if (versions != 0u) {
			send_versions();
		}

		if (nodes != 0u) {
			send_nodes();
		}

		cast_uint8_t(ISPU_IF2S_FLAG) = 0xFF; // reset flags
		cast_uint8_t(ISPU_INT_PIN) = cast_uint8_t(ISPU_INT_PIN) | 1u; // set INT1
	}
}

