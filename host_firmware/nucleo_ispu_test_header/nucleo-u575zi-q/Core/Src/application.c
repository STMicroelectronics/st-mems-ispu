/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "ispu.h"

#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

#define UART_BUF_SIZE 256

static void read(uint8_t reg, uint8_t *val, uint16_t len);
static void write(uint8_t reg, uint8_t val);
static void write_mul(uint8_t reg, uint8_t *val, uint16_t len);
uint8_t get_type_size(uint8_t type);

static void interrupt_callback(uint16_t GPIO_Pin, uint8_t rising);

static volatile char uart_char;
static volatile uint8_t uart_received;
static volatile char uart_buf[UART_BUF_SIZE + 1];
static volatile uint16_t uart_size;

static volatile uint8_t enable_int;
static volatile uint8_t algo_int;
static volatile uint8_t sleep_int;

static uint32_t exec_time, max_exec_time, min_exec_time;
static uint64_t num_exec_time;
static float avg_exec_time;

static uint16_t print_results;
static uint16_t print_time;

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

	print_results = 1;

	uint8_t valid_sensors[ISPU_CONF_SENSORS_NUM];

	for (uint32_t i = 0; i < ISPU_CONF_SENSORS_NUM; i++) {
		// check sensor name(s) validity
		uint32_t name_list_len = ispu_conf_name_lists[i].len;
		const char *const *name_list = ispu_conf_name_lists[i].list;

		valid_sensors[i] = 1;
		for (uint32_t j = 0; j < name_list_len; j++) {
			if (strcmp(name_list[j], "ISM330IS") != 0 && strcmp(name_list[j], "LSM6DSO16IS") != 0) {
				valid_sensors[i] = 0;
				break;
			}
		}
		if (!valid_sensors[i])
			continue;

		// load configuration
		uint32_t conf_len = ispu_conf_confs[i].len;
		const struct mems_conf_op *conf = ispu_conf_confs[i].list;

		uint32_t mem_i = 0;
		uint8_t *mem_buf = malloc(conf_len);

		if (mem_buf) { // fast loading
			uint8_t ispu_page = 0;
			for (uint32_t j = 0; j < conf_len; j++) {
				if (conf[j].type == MEMS_CONF_OP_TYPE_WRITE) {
					if (conf[j].address == 0x01) {
						if (conf[j].data & 0x80)
							ispu_page = 1;
						else
							ispu_page = 0;
					}
					uint8_t is_mem_write = ispu_page && conf[j].address == 0x0B;

					if (is_mem_write) {
						mem_buf[mem_i++] = conf[j].data;
					} else {
						if (mem_i > 0) {
							write_mul(0x0B, mem_buf, mem_i);
							mem_i = 0;
						}
						write(conf[j].address, conf[j].data);
					}
				} else if (conf[j].type == MEMS_CONF_OP_TYPE_DELAY) {
					HAL_Delay(conf[j].data);
				}
			}
		} else { // fallback to slow loading
			for (uint32_t j = 0; j < conf_len; j++) {
				if (conf[j].type == MEMS_CONF_OP_TYPE_WRITE)
					write(conf[j].address, conf[j].data);
				else if (conf[j].type == MEMS_CONF_OP_TYPE_DELAY)
					HAL_Delay(conf[j].data);
			}
		}

		// print header for output data
		uint32_t output_list_len = ispu_conf_output_lists[i].len;
		const struct mems_conf_output *output_list = ispu_conf_output_lists[i].list;

		for (uint32_t j = 0; j < output_list_len; j++) {
			// handle char arrays as len = 1 as they are strings
			if (output_list[j].len == 1 || output_list[j].type == MEMS_CONF_OUTPUT_TYPE_CHAR) {
				printf("%s", output_list[j].name);
			} else {
				for (uint32_t k = 0; k < output_list[j].len; k++) {
					printf("%s[%lu]", output_list[j].name, k);
					if (k < output_list[j].len  - 1)
						printf("\t");
				}
			}
			if (j < output_list_len - 1)
				printf("\t");
			else
				printf("\n");
		}
	}

	HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_char, 1);
	enable_int = 1;

	while (1) {
		// handle commands received from uart
		if (uart_received) {
			if (sscanf((char *)uart_buf, "res%hu", &print_results) > 0) {
				if (print_results)
					printf("Enabled results print.\n");
				else
					printf("Disabled results print.\n");
			}

			if (sscanf((char *)uart_buf, "time%hu", &print_time) > 0) {
				if (print_time)
					printf("Enabled execution time print.\n");
				else
					printf("Disabled execution time print.\n");
			}

			uint8_t reg, val;

			if (sscanf((char *)uart_buf, "write%02hhX%02hhX", &reg, &val) == 2) {
				write(reg, val);
				printf("wrote 0x%02hhX = 0x%02hhX\n", reg, val);
			}

			if (sscanf((char *)uart_buf, "read%02hhX", &reg) == 1) {
				read(reg, &val, 1);
				printf("read 0x%02hhX = 0x%02hhX\n", reg, val);
			}

			if (sscanf((char *)uart_buf, "write_ispu%02hhX%02hhX", &reg, &val) == 2) {
				write(0x01, 0x80);
				write(reg, val);
				write(0x01, 0x00);
				printf("wrote 0x%02hhX = 0x%02hhX\n", reg, val);
			}

			if (sscanf((char *)uart_buf, "read_ispu%02hhX", &reg) == 1) {
				write(0x01, 0x80);
				read(reg, &val, 1);
				write(0x01, 0x00);
				printf("read 0x%02hhX = 0x%02hhX\n", reg, val);
			}

			uart_size = 0;
			uart_received = 0;
		}

		if (algo_int) {
			algo_int = 0;

			// read ispu int status to clear interrupt if latched
			uint8_t ispu_int_status[4];
			read(0x1A, ispu_int_status, 4);

			if (print_results) {
				write(0x01, 0x80);

				// read and print outputs
				for (uint32_t i = 0; i < ISPU_CONF_SENSORS_NUM; i++) {
					if (!valid_sensors[i])
						continue;

					uint32_t output_list_len = ispu_conf_output_lists[i].len;
					const struct mems_conf_output *output_list = ispu_conf_output_lists[i].list;

					for (uint32_t j = 0; j < output_list_len; j++) {
						uint8_t out_type = output_list[j].type;
						uint8_t out_addr = output_list[j].reg_addr;
						uint8_t type_size = get_type_size(out_type);

						uint8_t value[8]; // max type size is 8 bytes

						for (uint32_t k = 0; k < output_list[j].len; k++) {
							read(out_addr, value, type_size);

							uint8_t early_end = 0;

							switch (out_type) {
							case MEMS_CONF_OUTPUT_TYPE_UINT8_T:
								printf("%u", *(uint8_t *)value);
#ifndef NO_LABELS
								uint8_t num_results = output_list[j].num_results;
								const struct mems_conf_result *results = output_list[j].results;

								for (uint32_t l = 0; l < num_results; l++) {
									if (*(uint8_t *)value == results[l].code)
										printf(" (%s)", results[l].label);
								}
#endif
								break;
							case MEMS_CONF_OUTPUT_TYPE_INT8_T:
								printf("%d", *(int8_t *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_CHAR:
								if (*(char *)value == '\0')
									early_end = 1;
								else
									printf("%c", *(char *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_UINT16_T:
								printf("%u", *(uint16_t *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_INT16_T:
								printf("%d", *(int16_t *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_UINT32_T:
								printf("%lu", *(uint32_t *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_INT32_T:
								printf("%ld", *(int32_t *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_UINT64_T:
								printf("%llu", *(uint64_t *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_INT64_T:
								printf("%lld", *(int64_t *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_HALF:
								printf("%f", *(__fp16 *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_FLOAT:
								printf("%f", *(float *)value);
								break;
							case MEMS_CONF_OUTPUT_TYPE_DOUBLE:
								printf("%f", *(double *)value);
								break;
							}

							if (early_end)
								break;

							if (k < output_list[j].len - 1 && out_type != MEMS_CONF_OUTPUT_TYPE_CHAR)
								printf("\t");

							out_addr += type_size;
						}
						if (j < output_list_len - 1)
							printf("\t");
						else
							printf("\n");
					}
				}
				write(0x01, 0x00);
			}
		}

		if (sleep_int) {
			sleep_int = 0;

			if (print_time)
				printf("%lu\t%lu\t%lu\t%f\n", exec_time, min_exec_time, max_exec_time, avg_exec_time);
		}
	}
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

	HAL_UART_Receive_IT(&huart1, (uint8_t *)&uart_char, 1);
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
		case INT1_Pin:
			algo_int = 1;
			break;
		case DIL24_INT2_Pin:
		case IKS4A1_LSM6DSO16IS_INT2_Pin:
			if (!rising) {
				__HAL_TIM_SET_COUNTER(&htim5, 0);
				HAL_TIM_Base_Start(&htim5);
			} else {
				HAL_TIM_Base_Stop(&htim5);
				exec_time = __HAL_TIM_GET_COUNTER(&htim5);

				if (num_exec_time++ == 1) { // discard first (will be algo init)
					avg_exec_time = exec_time;
					min_exec_time = UINT32_MAX;
					max_exec_time = 0;
				}

				avg_exec_time += (float)(exec_time - avg_exec_time) / (float)num_exec_time;
				if (exec_time < min_exec_time)
					min_exec_time = exec_time;
				if (exec_time > max_exec_time)
					max_exec_time = exec_time;

				sleep_int = 1;
			}
		}
	}
}

uint8_t get_type_size(uint8_t type)
{
	const struct {
		uint8_t id;
		uint8_t size;
	} types[] = {
		{ MEMS_CONF_OUTPUT_TYPE_UINT8_T,  1 },
		{ MEMS_CONF_OUTPUT_TYPE_INT8_T,   1 },
		{ MEMS_CONF_OUTPUT_TYPE_CHAR,     1 },
		{ MEMS_CONF_OUTPUT_TYPE_UINT16_T, 2 },
		{ MEMS_CONF_OUTPUT_TYPE_INT16_T,  2 },
		{ MEMS_CONF_OUTPUT_TYPE_UINT32_T, 4 },
		{ MEMS_CONF_OUTPUT_TYPE_INT32_T,  4 },
		{ MEMS_CONF_OUTPUT_TYPE_UINT64_T, 8 },
		{ MEMS_CONF_OUTPUT_TYPE_INT64_T,  8 },
		{ MEMS_CONF_OUTPUT_TYPE_HALF,     2 },
		{ MEMS_CONF_OUTPUT_TYPE_FLOAT,    4 },
		{ MEMS_CONF_OUTPUT_TYPE_DOUBLE,   8 }
	};

	for (uint8_t i = 0; i < sizeof(types) / sizeof(types[0]); i++) {
		if (type == types[i].id)
			return types[i].size;
	}

	return 0;
}

