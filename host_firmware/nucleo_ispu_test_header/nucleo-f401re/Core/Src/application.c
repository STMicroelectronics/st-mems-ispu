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

struct device {
	char name[100];
	uint8_t n_dev_names;
	char dev_names[2][100];
	uint8_t n_i2c_addr;
	uint8_t i2c_addr[2];
	uint8_t who_am_i;
	uint8_t ram_data;
};

static struct device *get_device(char *name);
static void set_i2c_addr(uint8_t addr);
static void read(uint8_t reg, uint8_t *val, uint32_t len);
static void write(uint8_t reg, uint8_t val);
static void write_mul(uint8_t reg, uint8_t *val, uint32_t len);
uint8_t get_type_size(uint8_t type);

struct device devices[] = {
	{ // ISM330IS and LSM6DSO16IS
		.name = "imu22",
		.n_dev_names = 2,
		.dev_names = { "ISM330IS", "LSM6DSO16IS" },
		.n_i2c_addr = 2,
		.i2c_addr = { 0xD4, 0xD6 },
		.who_am_i = 0x22,
		.ram_data = 0x0B
	}
};

static struct device *dev;
static uint8_t i2c_addr;

static volatile char uart_char;
static volatile uint8_t uart_received;
static volatile char uart_buf[UART_BUF_SIZE + 1];
static volatile uint16_t uart_size;

static volatile uint8_t enable_int;
static volatile uint8_t algo_int;
static volatile uint8_t sleep_int;

static volatile uint32_t exec_time, max_exec_time, min_exec_time;
static volatile uint64_t num_exec_time;
static volatile float avg_exec_time;

static uint16_t print_results;
static uint16_t print_time;

void application(void)
{
	HAL_Delay(1000);

	int8_t sensors[ISPU_CONF_SENSORS_NUM];

	for (uint32_t i = 0; i < ISPU_CONF_SENSORS_NUM; i++) {
		// check sensor name(s) validity
		uint32_t name_list_len = ispu_conf_name_lists[i].len;
		const char *const *name_list = ispu_conf_name_lists[i].list;

		sensors[i] = -1;

		for (uint32_t j = 0; j < name_list_len; j++) {
			for (uint8_t k = 0; k < sizeof(devices) / sizeof(devices[0]); k++) {
				for (uint32_t l = 0; l < devices[k].n_dev_names; l++) {
					if (strcasecmp(name_list[j], devices[k].dev_names[l]) == 0) {
						sensors[i] = k;
						goto name_search_end;
					}
				}
			}
		}

		name_search_end:

		if (sensors[i] == -1) {
			while (1) {
				printf("Error: sensor ");
				for (uint32_t j = 0; j < name_list_len; j++) {
					if (j > 0)
						printf(" / ");
					printf("%s", name_list[j]);
				}
				printf(" unknown\n");

				HAL_Delay(1000);
			}
		}

		if (dev == NULL) {
			dev = get_device(devices[sensors[i]].name);

			if (dev == NULL) {
				while (1) {
					printf("Error: sensor ");
					for (uint8_t j = 0; j < devices[sensors[i]].n_dev_names; j++) {
						if (j > 0)
							printf(" / ");
						printf("%s", devices[sensors[i]].dev_names[j]);
					}
					printf(" not found\n");

					HAL_Delay(1000);
				}
			}

			// software reset
			uint8_t tmp;
			read(0x12, &tmp, 1);
			write(0x12, tmp | 0x01);
			do {
				read(0x12, &tmp, 1);
			} while ((tmp & 0x01) != 0);

			HAL_Delay(2);
		} else {
			if (devices[sensors[i]].name != dev->name) {
				while (1) {
					printf("Error: cannot configure multiple sensors\n");
					HAL_Delay(1000);
				}
			}
		}
	}

	uint8_t outputs = 0;

	for (uint32_t i = 0; i < ISPU_CONF_SENSORS_NUM; i++) {
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
					uint8_t is_mem_write = ispu_page && conf[j].address == dev->ram_data;

					if (is_mem_write) {
						mem_buf[mem_i++] = conf[j].data;
					} else {
						if (mem_i > 0) {
							write_mul(dev->ram_data, mem_buf, mem_i);
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
			if (outputs)
				printf("\t");

			outputs = 1;

			// handle char arrays as len = 1 as they are strings
			if (output_list[j].len == 1 || output_list[j].type == MEMS_CONF_OUTPUT_TYPE_CHAR) {
				printf("%s", output_list[j].name);
			} else {
				for (uint32_t k = 0; k < output_list[j].len; k++) {
					if (k > 0)
						printf("\t");
					printf("%s[%lu]", output_list[j].name, k);
				}
			}
		}
	}

	if (outputs)
		printf("\n");

	HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_char, 1);
	enable_int = 1;
	print_results = 1;

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

				outputs = 0;

				// read and print outputs
				for (uint32_t i = 0; i < ISPU_CONF_SENSORS_NUM; i++) {
					if (sensors[i] == -1)
						continue;

					uint32_t output_list_len = ispu_conf_output_lists[i].len;
					const struct mems_conf_output *output_list = ispu_conf_output_lists[i].list;

					for (uint32_t j = 0; j < output_list_len; j++) {
						uint8_t out_type = output_list[j].type;
						uint8_t out_addr = output_list[j].reg_addr;
						uint8_t type_size = get_type_size(out_type);

						uint8_t value[8]; // max type size is 8 bytes

						if (outputs)
							printf("\t");

						outputs = 1;

						for (uint32_t k = 0; k < output_list[j].len; k++) {
							read(out_addr, value, type_size);

							uint8_t early_end = 0;

							if (k > 0 && out_type != MEMS_CONF_OUTPUT_TYPE_CHAR)
								printf("\t");

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

							out_addr += type_size;
						}
					}
				}
				if (outputs)
					printf("\n");

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

static struct device *get_device(char *name)
{
	struct device *dev = NULL;

	for (uint8_t i = 0; i < sizeof(devices) / sizeof(devices[0]); i++) {
		dev = &devices[i];

		if (strcmp(name, dev->name) != 0) {
			dev = NULL;
			continue;
		}

		uint8_t who_am_i = 0x00;

		for (uint8_t j = 0; j < dev->n_i2c_addr; j++) {
			set_i2c_addr(dev->i2c_addr[j]);

			uint32_t start = HAL_GetTick();
			do {
				if (HAL_GetTick() - start > 100)
					break;
				write(0x01, 0x00); // set default registers access
				read(0x0F, &who_am_i, 1);
			} while (who_am_i != dev->who_am_i);

			if (who_am_i == dev->who_am_i)
				break;
		}

		if (who_am_i == dev->who_am_i)
			break;
		else
			dev = NULL;
	}

	return dev;
}

static void set_i2c_addr(uint8_t addr)
{
	i2c_addr = addr;
}

static void read(uint8_t reg, uint8_t *val, uint32_t len)
{
	HAL_I2C_Mem_Read(&hi2c1, i2c_addr, reg, I2C_MEMADD_SIZE_8BIT, val, len, HAL_MAX_DELAY);
}

static void write(uint8_t reg, uint8_t val)
{
	HAL_I2C_Mem_Write(&hi2c1, i2c_addr, reg, I2C_MEMADD_SIZE_8BIT, &val, 1, HAL_MAX_DELAY);
}

static void write_mul(uint8_t reg, uint8_t *val, uint32_t len)
{
	HAL_I2C_Mem_Write(&hi2c1, i2c_addr, reg, I2C_MEMADD_SIZE_8BIT, val, len, HAL_MAX_DELAY);
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

	HAL_UART_Receive_IT(&huart2, (uint8_t *)&uart_char, 1);
}

int _write(int fd, const void *buf, size_t count)
{
	uint8_t status = HAL_UART_Transmit(&huart2, (uint8_t *)buf, count, HAL_MAX_DELAY);

	return (status == HAL_OK ? count : 0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	GPIO_TypeDef *int2_port;
	uint16_t int2_pin;

	if (enable_int) {
		switch (GPIO_Pin) {
		case INT1_Pin:
			algo_int = 1;
			break;
		case DIL24_INT2_Pin:
		case IKS4A1_LSM6DSO16IS_INT2_Pin:
			if (GPIO_Pin == DIL24_INT2_Pin) {
				int2_port = DIL24_INT2_GPIO_Port;
				int2_pin = DIL24_INT2_Pin;
			} else {
				int2_port = IKS4A1_LSM6DSO16IS_INT2_GPIO_Port;
				int2_pin = IKS4A1_LSM6DSO16IS_INT2_Pin;
			}
			if (HAL_GPIO_ReadPin(int2_port, int2_pin) == GPIO_PIN_RESET) {
				__HAL_TIM_SET_COUNTER(&htim5, 0);
				HAL_TIM_Base_Start(&htim5);
			} else if (HAL_GPIO_ReadPin(int2_port, int2_pin) == GPIO_PIN_SET) {
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

