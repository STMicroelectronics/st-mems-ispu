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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "application.h"

#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#define UART_BUF_SIZE 256
#define BUF_SIZE (1024 * 4)
#define BUF_N 2

#define TIMEOUT_BOOT (1000000 * 5)
#define TIMEOUT_RUN (1000000 * 30)

#define CANARY 0xA5
#define CANARY_ADDR 0x4C

#define TIMEOUT_ERROR 0x00000001
#define STACK_ERROR 0x00000002
#define UCF_VERSION_ERROR 0x00000004

#define LAYERS_BUF_SIZE 256

static const uint8_t expected_ucf_version[2] = { 1, 1 };

union mem {
	uint8_t bytes[4];
	uint32_t value;
};

static void interrupt_callback(uint16_t GPIO_Pin, uint8_t rising);

static void signal_ispu();
static void wait_ispu();
static void get_and_send_info();

static uint8_t stack_overflow();
static void read(uint8_t reg, uint8_t *val, uint16_t len);
static void write(uint8_t reg, uint8_t val);
static void write_mul(uint8_t reg, uint8_t *val, uint16_t len);

static volatile char uart_char;
static volatile uint8_t uart_received;
static volatile char uart_buf[UART_BUF_SIZE + 1];
static volatile uint16_t uart_size;

static uint32_t size;
static uint8_t buf_i, buf_i_read;
static uint32_t block_size, block_size_read[2];
static uint8_t buf[BUF_N][BUF_SIZE];
static volatile uint8_t received;
static volatile uint8_t send_pending;
static volatile uint8_t done;
static uint8_t receiving_ucf, receiving_input;

static uint8_t ucf_sunchon_page;
static uint8_t ucf_mem_buf[BUF_SIZE / 2];

static uint32_t input_idx;
static uint32_t input_offset;

static volatile uint8_t enable_int;
static volatile uint8_t ispu_int;
static volatile uint8_t layer_int;
static uint8_t waiting_interrupt;
static uint32_t exec_time, layer_start, layer_time[LAYERS_BUF_SIZE];
static uint16_t layer_next, layer_curr;
static uint8_t first_exec_int;

static uint32_t timeout_boot = TIMEOUT_BOOT;
static uint32_t timeout_run = TIMEOUT_RUN;

void application(void)
{
	HAL_Delay(1000);

	uint8_t who_am_i;
	uint32_t start = HAL_GetTick();
	do {
		if (HAL_GetTick() - start > 1000) { // retry for 1.0 s
			while (1) {
				printf("Error: sensor not recognized (%02x)\n", who_am_i);
				HAL_Delay(1000);
			}
		}
		write(0x01, 0x00); // set default registers access
		read(0x0F, &who_am_i, 1);
	} while (who_am_i != 0x22);

	// software reset
	uint8_t tmp;
	read(0x12, &tmp, 1);
	write(0x12, tmp | 0x01);
	do {
		read(0x12, &tmp, 1);
	} while ((tmp & 0x01) != 0);

	HAL_TIM_Base_Start(&htim2);
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_char, 1);

	enable_int = 1;

	while (1) {
		// handle commands received from uart
		if (uart_received) {
			uart_size = 0;
			uart_received = 0;

			if (strcmp((char *)uart_buf, "ver") == 0) {
				printf("ISPU validation firmware 1.1.1\n");
			} else if (strcmp((char *)uart_buf, "name") == 0) {
				char name[1024];

				write(0x01, 0x80);
				write(0x02, 0x03); // disable the clock

				union mem name_addr;
				read(0x10 + 4, name_addr.bytes, 4);
				write(0x08, name_addr.bytes[2] | 0x40); // set read mode and ram address
				write(0x09, name_addr.bytes[1]); // set ram address
				write(0x0A, name_addr.bytes[0]); // set ram address
				read(0x0B, (uint8_t *)name, 1); // discard bad byte

				uint16_t i = 0;
				do {
					read(0x0B, (uint8_t *)&name[i], 1);
					i++;
				} while(name[i - 1] != '\0');

				write(0x02, 0x01); // enable the clock
				write(0x01, 0x00);

				printf("%s\n", name);
			} else if (strcmp((char *)uart_buf, "clock") == 0) {
				uint8_t ctrl10 = 0;
				read(0x19, &ctrl10, 1);

				uint32_t freq = 5000000;
				if (ctrl10 & 0x4)
					freq = 10000000;

				printf("%lu\n", freq);
			} else if (strcmp((char *)uart_buf, "macc") == 0) {
				uint64_t macc = 0;

				write(0x01, 0x80);
				read(0x10 + 22, (uint8_t *)&macc, 8);
				write(0x01, 0x00);

				printf("%llu\n", macc);
			} else if (strcmp((char *)uart_buf, "blocksize") == 0) {
				printf("%u\n", BUF_SIZE);
			} else if (strncmp((char *)uart_buf, "timeout_boot", 12) == 0) {
				sscanf((char *)uart_buf, "timeout_boot%lu", &timeout_boot);
				timeout_boot *= 1000000;
				printf("Timeout boot = %lu", timeout_boot);
			} else if (strncmp((char *)uart_buf, "timeout_run", 11) == 0) {
				sscanf((char *)uart_buf, "timeout_run%lu", &timeout_run);
				timeout_run *= 1000000;
				printf("Timeout run = %lu", timeout_run);
			} else if (strcmp((char *)uart_buf, "") == 0) {
				printf("%u\n", BUF_SIZE);
			} else if (strcmp((char *)uart_buf, "canary") == 0) {
				uint8_t canary = 0x00;

				write(0x01, 0x80);
				write(0x02, 0x03); // disable the clock

				union mem canary_addr;
				read(CANARY_ADDR, canary_addr.bytes, 4);
				write(0x08, canary_addr.bytes[2] | 0x40); // set read mode and ram address
				write(0x09, canary_addr.bytes[1]); // set ram address
				write(0x0A, canary_addr.bytes[0]); // set ram address
				read(0x0B, &canary, 1); // discard bad byte

				read(0x0B, &canary, 1);

				write(0x02, 0x01); // enable the clock
				write(0x01, 0x00);

				printf("0x%lX = 0x%02X\n", canary_addr.value, canary);
			} else if (strcmp((char *)uart_buf, "get_in_info") == 0) {
				write(0x01, 0x80);
				write(0x0C, 0x02); // give input info command to the ISPU
				write(0x01, 0x00);
				write(0x18, 0xA0); // wake up the ISPU

				get_and_send_info(); // get info from the ISPU and send over uart
			} else if (strcmp((char *)uart_buf, "get_out_info") == 0) {
				write(0x01, 0x80);
				write(0x0C, 0x04); // give output info command to the ISPU
				write(0x01, 0x00);
				write(0x18, 0xA0); // wake up the ISPU

				get_and_send_info(); // get info from the ISPU and send over uart
			} else if (strcmp((char *)uart_buf, "activation_sizes") == 0) {
				write(0x01, 0x80);

				uint8_t reg = 0x10 + 30; // after macc
				uint8_t num_act = 0;

				read(reg, &num_act, 1);
				reg += 1;

				printf("%u\n", num_act);

				union mem activations_size_addr;
				read(reg, activations_size_addr.bytes, 4);

				for (uint8_t i = 0; i < num_act; i++) {
					uint32_t activation_size = 0;

					write(0x08, activations_size_addr.bytes[2] | 0x40); // set read mode and ram address
					write(0x09, activations_size_addr.bytes[1]); // set ram address
					write(0x0A, activations_size_addr.bytes[0]); // set ram address

					read(0x0B, (uint8_t *)&activation_size, 1); // discard bad byte
					read(0x0B, (uint8_t *)&activation_size, 4);

					printf("%lu\n", activation_size);

					activations_size_addr.value += 4;
				}

				write(0x01, 0x00);
			} else if (strcmp((char *)uart_buf, "weight_sizes") == 0) {
				write(0x01, 0x80);

				uint8_t reg = 0x10 + 35; // after activations
				uint8_t num_weight = 0;

				read(reg, &num_weight, 1);
				reg += 1;

				printf("%u\n", num_weight);

				union mem weights_size_addr;
				read(reg, weights_size_addr.bytes, 4);

				for (uint8_t i = 0; i < num_weight; i++) {
					uint32_t weight_size = 0;

					write(0x08, weights_size_addr.bytes[2] | 0x40); // set read mode and ram address
					write(0x09, weights_size_addr.bytes[1]); // set ram address
					write(0x0A, weights_size_addr.bytes[0]); // set ram address

					read(0x0B, (uint8_t *)&weight_size, 1); // discard bad byte
					read(0x0B, (uint8_t *)&weight_size, 4);

					printf("%lu\n", weight_size);

					weights_size_addr.value += 4;
				}

				write(0x01, 0x00);
			} else if (strcmp((char *)uart_buf, "get_versions") == 0) {
				write(0x01, 0x80);
				write(0x0C, 0x08); // give versions command to the ISPU
				write(0x01, 0x00);
				write(0x18, 0xA0); // wake up the ISPU

				write(0x01, 0x80);

				for (uint8_t i = 0; i < 6; i++) {
					wait_ispu();
					uint8_t ver = 0;
					read(0x10, (uint8_t *)&ver, 1);
					printf("%u\n", ver);
					signal_ispu();
				}

				write(0x01, 0x00);
			} else if (strcmp((char *)uart_buf, "get_nodes") == 0) {
				write(0x01, 0x80);
				write(0x0C, 0x10); // give nodes command to the ISPU
				write(0x01, 0x00);
				write(0x18, 0xA0); // wake up the ISPU

				write(0x01, 0x80);

				wait_ispu();
				uint32_t n_nodes = 0;
				read(0x10, (uint8_t *)&n_nodes, 4);
				printf("%lu\n", n_nodes);
				signal_ispu();

				for (uint32_t i = 0; i < n_nodes; i++) {
					wait_ispu();
					int32_t id = 0;
					read(0x10, (uint8_t *)&id, 4);
					printf("%ld\n", id);
					signal_ispu();

					wait_ispu();
					int16_t type = 0;
					read(0x10, (uint8_t *)&type, 2);
					printf("%d\n", type);
					signal_ispu();

					get_and_send_info(); // input tensors
					get_and_send_info(); // output tensors
				}

				write(0x01, 0x00);
			} else if (strcmp((char *)uart_buf, "run") == 0) {
				HAL_TIM_Base_Start(&htim5);

				first_exec_int = 1;
				waiting_interrupt = 1;

				layer_next = layer_curr = 0;

				printf("Requested execution.\n");

				write(0x01, 0x80);
				write(0x0C, 0x01); // give run command to ISPU
				write(0x01, 0x00);
				write(0x18, 0xA0); // wake up the ISPU

				__HAL_TIM_SET_COUNTER(&htim2, 0);
			}
		}

		if (received) {
			if (receiving_ucf) {
				uint32_t mem_i = 0;

				for (uint32_t i = 0; i < block_size_read[buf_i_read]; i += 2) {
					if (buf[buf_i_read][i] == 0x01) {
						if (buf[buf_i_read][i + 1] & 0x80)
							ucf_sunchon_page = 1;
						else
							ucf_sunchon_page = 0;
					} else if (buf[buf_i_read][i] == 0xFF) {
						HAL_Delay(buf[buf_i_read][i + 1]);
					}

					uint8_t is_mem_write = ucf_sunchon_page && buf[buf_i_read][i] == 0x0B;
					uint8_t is_last_write = i == block_size_read[buf_i_read] - 2;

					if (is_mem_write && !is_last_write) {
						ucf_mem_buf[mem_i++] = buf[buf_i_read][i + 1];
					} else if (is_mem_write && is_last_write) {
						ucf_mem_buf[mem_i++] = buf[buf_i_read][i + 1];
						write_mul(0x0B, ucf_mem_buf, mem_i);
						mem_i = 0;
					} else {
						if (mem_i > 0) {
							write_mul(0x0B, ucf_mem_buf, mem_i);
							mem_i = 0;
						}
						write(buf[buf_i_read][i], buf[buf_i_read][i + 1]);
					}
				}
			} else if (receiving_input) {
				write(0x01, 0x80);
				write(0x02, 0x03); // disable the clock

				uint8_t reg = 0x10 + 8; // start after name address
				uint8_t num_in = 0;

				read(reg, &num_in, 1);
				reg += 1;

				union mem inputs_buf_addr;
				read(reg, inputs_buf_addr.bytes, 4);

				inputs_buf_addr.value += input_idx * 4;

				write(0x08, inputs_buf_addr.bytes[2] | 0x40); // set read mode and ram address
				write(0x09, inputs_buf_addr.bytes[1]); // set ram address
				write(0x0A, inputs_buf_addr.bytes[0]); // set ram address

				union mem inputs_addr;
				read(0x0B, inputs_addr.bytes, 1); // discard bad byte
				read(0x0B, inputs_addr.bytes, 4);
				inputs_addr.value += input_offset;

				write(0x08, inputs_addr.bytes[2] | 0x00); // set write mode for data ram
				write(0x09, inputs_addr.bytes[1]); // set ram address
				write(0x0A, inputs_addr.bytes[0]); // set ram address
				write_mul(0x0B, buf[buf_i_read], block_size_read[buf_i_read]);

				input_offset += block_size_read[buf_i_read];

				write(0x02, 0x01); // enable the clock
				write(0x01, 0x00);
			}

			buf_i_read = (buf_i_read + 1) % BUF_N;
			received--;

			if (send_pending) {
				send_pending = 0;
				HAL_UART_Receive_DMA(&huart1, buf[buf_i], block_size);
				printf("Ready to receive block of size %lu\n", block_size);
			}

			if (received == 0 && done) {
				printf("Loading completed.\n");

				if (receiving_ucf) {
					uint8_t tmp, timeout = 0;
					__HAL_TIM_SET_COUNTER(&htim2, 0);

					write(0x01, 0x80);

					do {
						if (__HAL_TIM_GET_COUNTER(&htim2) > timeout_boot) {
							timeout = 1;
							break;
						}
						read(0x04, &tmp, 1);
					} while ((tmp & 0x04) == 0x00);

					write(0x01, 0x00);

					uint32_t return_code = 0;
					if (timeout)
						return_code |= TIMEOUT_ERROR;
					if (stack_overflow())
						return_code |= STACK_ERROR;

					uint8_t version[3] = { 0, 0, 0 };
					if (return_code == 0) {
						write(0x01, 0x80);
						read(0x49, version, 3);
						write(0x01, 0x00);
						if (version[0] != expected_ucf_version[0] || version[1] != expected_ucf_version[1])
							return_code |= UCF_VERSION_ERROR;
					}

					printf("0x%lX\n", return_code);

					if (return_code & UCF_VERSION_ERROR) {
						printf("%u.%u.%u\n", version[0], version[1], version[2]);
						printf("%u.%u\n", expected_ucf_version[0], expected_ucf_version[1]);
					}
				}
			}
		}

		if (ispu_int && !waiting_interrupt)
			ispu_int = 0; // ignore if interrupt arrives after timeout

		if (layer_int) {
			layer_int = 0;
			while (layer_curr != layer_next) {
				printf("%lu\n", layer_time[layer_curr]);
				layer_curr = (layer_curr + 1) % LAYERS_BUF_SIZE;
			}
		}

		if (ispu_int) {
			ispu_int = 0;
			waiting_interrupt = 0;

			write(0x18, 0x00); // turn off the ISPU
			HAL_Delay(1); // make sure the ISPU is sleeping

			uint32_t return_code = 0;
			if (stack_overflow())
				return_code |= STACK_ERROR;

			printf("ret 0x%lX\n", return_code);

			if (return_code == 0) {
				write(0x01, 0x80);
				write(0x02, 0x03); // disable the clock

				uint32_t ret;
				read(0x10, (uint8_t *)&ret, sizeof(ret));
				printf("%lu\n", exec_time);
				HAL_TIM_Base_Stop(&htim5);

				uint8_t reg = 0x10 + 13; // start after inputs
				uint8_t num_out = 0;

				read(reg, &num_out, 1);
				reg += 1;

				union mem outputs_buf_addr;
				read(reg, outputs_buf_addr.bytes, 4);
				reg += 4;

				union mem outputs_size_addr;
				read(reg, outputs_size_addr.bytes, 4);

				for (uint8_t i = 0; i < num_out; i++) {
					union mem outputs_addr;
					uint32_t output_size = 0;

					write(0x08, outputs_buf_addr.bytes[2] | 0x40); // set read mode and ram address
					write(0x09, outputs_buf_addr.bytes[1]); // set ram address
					write(0x0A, outputs_buf_addr.bytes[0]); // set ram address

					read(0x0B, outputs_addr.bytes, 1); // discard bad byte
					read(0x0B, outputs_addr.bytes, 4);
					outputs_buf_addr.value += 4;

					write(0x08, outputs_size_addr.bytes[2] | 0x40); // set read mode and ram address
					write(0x09, outputs_size_addr.bytes[1]); // set ram address
					write(0x0A, outputs_size_addr.bytes[0]); // set ram address

					read(0x0B, (uint8_t *)&output_size, 1); // discard bad byte
					read(0x0B, (uint8_t *)&output_size, 4);
					outputs_size_addr.value += 4;

					while (output_size > 0) {
						uint32_t chunk_size = output_size > BUF_SIZE ? BUF_SIZE : output_size;

						write(0x08, outputs_addr.bytes[2] | 0x40); // set read mode from data ram
						write(0x09, outputs_addr.bytes[1]); // set ram address
						write(0x0A, outputs_addr.bytes[0]); // set ram address
						read(0x0B, buf[0], 1); // discard bad byte
						read(0x0B, buf[0], chunk_size);

						HAL_UART_Transmit(&huart1, buf[0], chunk_size, 1000);

						outputs_addr.value += chunk_size;
						output_size -= chunk_size;
					}
				}
				write(0x02, 0x01); // enable the clock
				write(0x01, 0x00);
			}
		} else if (waiting_interrupt) {
			if (__HAL_TIM_GET_COUNTER(&htim2) > timeout_run) {
				waiting_interrupt = 0;

				uint32_t return_code = TIMEOUT_ERROR;
				if (stack_overflow())
					return_code |= STACK_ERROR;

				printf("ret 0x%lX\n", return_code);
			}
		}
	}
}

static void signal_ispu()
{
	write(0x0E, 0x01);
}

static void wait_ispu()
{
	uint8_t tmp = 0;
	do {
		read(0x0E, &tmp, 1);
	} while (tmp == 0x00);
}

static void get_and_send_info()
{
	write(0x01, 0x80);

	wait_ispu();
	uint16_t num = 0;
	read(0x10, (uint8_t *)&num, 2);
	printf("%u\n", num);
	signal_ispu();

	for (uint8_t i = 0; i < num; i++) {
		wait_ispu();
		uint32_t format = 0;
		read(0x10, (uint8_t *)&format, 4);
		printf("0x%lX\n", format);
		signal_ispu();

		uint32_t size = 0;

		wait_ispu();
		read(0x10, (uint8_t *)&size, 4);
		printf("%lu\n", size);
		signal_ispu();
		for (uint8_t j = 0; j < size; j++) {
			wait_ispu();
			int32_t dim = 0;
			read(0x10, (uint8_t *)&dim, 4);
			printf("%ld\n", dim);
			signal_ispu();
		}

		wait_ispu();
		read(0x10, (uint8_t *)&size, 4);
		printf("%lu\n", size);
		signal_ispu();
		for (uint8_t j = 0; j < size; j++) {
			wait_ispu();
			float scale = 0;
			read(0x10, (uint8_t *)&scale, 4);
			HAL_UART_Transmit(&huart1, (uint8_t *)&scale, 4, 1000);
			signal_ispu();
		}

		wait_ispu();
		read(0x10, (uint8_t *)&size, 4);
		printf("%lu\n", size);
		signal_ispu();
		for (uint8_t j = 0; j < size; j++) {
			wait_ispu();
			int16_t zeropoint = 0;
			read(0x10, (uint8_t *)&zeropoint, 2);
			printf("%d\n", zeropoint);
			signal_ispu();
		}

		wait_ispu();
		read(0x10, (uint8_t *)&size, 4);
		printf("%lu\n", size);
		signal_ispu();
		if (size > 0) {
			char name[4096] = "";
			for (uint32_t j = 0; j < size; j += 4) {
				wait_ispu();
				read(0x10, (uint8_t *)&name[j], 4);
				signal_ispu();
			}
			name[size] = '\0';
			printf("%s\n", name);
		}
	}

	write(0x01, 0x80);
}

static uint8_t stack_overflow()
{
	uint8_t canary = CANARY;

	write(0x01, 0x80);
	write(0x02, 0x03); // disable the clock

	union mem canary_addr;
	read(CANARY_ADDR, canary_addr.bytes, 4);
	write(0x08, canary_addr.bytes[2] | 0x40); // set read mode and ram address
	write(0x09, canary_addr.bytes[1]); // set ram address
	write(0x0A, canary_addr.bytes[0]); // set ram address
	read(0x0B, &canary, 1); // discard bad byte

	read(0x0B, &canary, 1);

	write(0x02, 0x01); // enable the clock
	write(0x01, 0x00);

	return canary != CANARY;
}

static void read(uint8_t reg, uint8_t *val, uint16_t len)
{
	HAL_I2C_Mem_Read(&hi2c1, 0xD4, reg, I2C_MEMADD_SIZE_8BIT, val, len, HAL_MAX_DELAY);
}

static void write(uint8_t reg, uint8_t val)
{
	HAL_I2C_Mem_Write(&hi2c1, 0xD4, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, HAL_MAX_DELAY);
}

static void write_mul(uint8_t reg, uint8_t *val, uint16_t len)
{
	HAL_I2C_Mem_Write(&hi2c1, 0xD4, reg, I2C_MEMADD_SIZE_8BIT, val, len, HAL_MAX_DELAY);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (size > 0) {
		block_size_read[buf_i] = block_size;

		size -= block_size;
		if (size == 0) {
			done = 1;
			received++;

			HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_char, 1);
		} else {
			received++;

			buf_i = (buf_i + 1) % BUF_N;
			block_size = size > BUF_SIZE ? BUF_SIZE : size;

			if (received < BUF_N) {
				HAL_UART_Receive_DMA(&huart1, buf[buf_i], block_size);
				printf("Ready to receive block of size %lu\n", block_size);
			} else {
				send_pending = 1;
			}
		}

		return;
	} else {
		receiving_ucf = 0;
		receiving_input = 0;
	}

	if (uart_char == '\n') {
		uart_buf[uart_size] = '\0';
		uart_received = 1;
	} else if (uart_char == '*') {
		uart_size = 0;
	} else if (uart_char != '\r') {
		if (uart_size >= UART_BUF_SIZE)
			uart_size = 0;
		uart_buf[uart_size++] = uart_char;
	}

	if (uart_received && strncmp((char *)uart_buf, "ucf", 3) == 0) {
		uart_size = 0;
		uart_received = 0;

		receiving_ucf = 1;
		ucf_sunchon_page = 0;

		buf_i = 0;
		buf_i_read = 0;
		done = 0;

		sscanf((char *)uart_buf, "ucf%lu", &size);

		block_size = size > BUF_SIZE ? BUF_SIZE : size;
		HAL_UART_Receive_DMA(&huart1, buf[buf_i], block_size);

		printf("Ready to receive block of size %lu\n", block_size);
	} else if (uart_received && strncmp((char *)uart_buf, "in", 2) == 0) {
		uart_size = 0;
		uart_received = 0;

		receiving_input = 1;
		input_offset = 0;

		buf_i = 0;
		buf_i_read = 0;
		done = 0;

		sscanf((char *)uart_buf, "in%lu%lu", &input_idx, &size);

		block_size = size > BUF_SIZE ? BUF_SIZE : size;
		HAL_UART_Receive_DMA(&huart1, buf[buf_i], block_size);

		printf("Ready to receive block of size %lu\n", block_size);
	} else {
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_char, 1);
	}

	return;
}

int _write(int fd, const void *buf, size_t count)
{
	uint8_t status = HAL_UART_Transmit(&huart1, (uint8_t *)buf, count, HAL_MAX_DELAY);

	return (status == HAL_OK ? count : 0);
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	interrupt_callback(GPIO_Pin, 1);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	interrupt_callback(GPIO_Pin, 0);
}

static void interrupt_callback(uint16_t GPIO_Pin, uint8_t rising)
{
	if (enable_int) {
		switch (GPIO_Pin) {
		case DIL24_INT2_Pin:
		case IKS4A1_LSM6DSO16IS_INT2_Pin:
			if (waiting_interrupt) {
				if (!rising) {
					if (first_exec_int) {
						__HAL_TIM_SET_COUNTER(&htim5, 0);
						first_exec_int = 0;
					} else {
						layer_time[layer_next++] = __HAL_TIM_GET_COUNTER(&htim5) - layer_start;
						layer_int = 1;
					}
				} else {
					layer_start = __HAL_TIM_GET_COUNTER(&htim5);
					exec_time = layer_start;
				}
			}
			break;
		case INT1_Pin:
			ispu_int = 1;
			break;
		}
	}
}

