/**
  ******************************************************************************
  * @file    network.h
  * @date    2025-02-05T14:48:06+0100
  * @brief   ST.AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */
#ifndef STAI_NETWORK_DETAILS_H
#define STAI_NETWORK_DETAILS_H

#include "stai.h"
#include "layers.h"

const stai_network_details g_network_details = {
  .tensors = (const stai_tensor[15]) {
   { .size_bytes = 1248, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 104, 3}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "input_output" },
   { .size_bytes = 3328, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 104, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "conv_hidden_0_conv2d_output" },
   { .size_bytes = 3328, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 104, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "conv_hidden_0_output" },
   { .size_bytes = 3328, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 104, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "batch_normalization_output" },
   { .size_bytes = 1664, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 52, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "max_pooling1d_output" },
   { .size_bytes = 1664, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 52, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "conv_hidden_1_conv2d_output" },
   { .size_bytes = 1664, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 52, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "conv_hidden_1_output" },
   { .size_bytes = 1664, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 52, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "batch_normalization_1_output" },
   { .size_bytes = 832, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 26, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "max_pooling1d_1_output" },
   { .size_bytes = 832, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 26, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "conv_hidden_2_conv2d_output" },
   { .size_bytes = 832, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 26, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "conv_hidden_2_output" },
   { .size_bytes = 832, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 26, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "batch_normalization_2_output" },
   { .size_bytes = 416, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {3, (const int32_t[3]){1, 13, 8}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "max_pooling1d_2_output" },
   { .size_bytes = 16, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {2, (const int32_t[2]){1, 4}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "dense_out_output" },
   { .size_bytes = 16, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {2, (const int32_t[2]){1, 4}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "output_output" }
  },
  .nodes = (const stai_node_details[14]){
    {.id = 1, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){0}}, .output_tensors = {1, (const int32_t[1]){1}} }, /* conv_hidden_0_conv2d */
    {.id = 1, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){1}}, .output_tensors = {1, (const int32_t[1]){2}} }, /* conv_hidden_0 */
    {.id = 2, .type = AI_LAYER_BN_TYPE, .input_tensors = {1, (const int32_t[1]){2}}, .output_tensors = {1, (const int32_t[1]){3}} }, /* batch_normalization */
    {.id = 3, .type = AI_LAYER_POOL_TYPE, .input_tensors = {1, (const int32_t[1]){3}}, .output_tensors = {1, (const int32_t[1]){4}} }, /* max_pooling1d */
    {.id = 4, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){4}}, .output_tensors = {1, (const int32_t[1]){5}} }, /* conv_hidden_1_conv2d */
    {.id = 4, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){5}}, .output_tensors = {1, (const int32_t[1]){6}} }, /* conv_hidden_1 */
    {.id = 5, .type = AI_LAYER_BN_TYPE, .input_tensors = {1, (const int32_t[1]){6}}, .output_tensors = {1, (const int32_t[1]){7}} }, /* batch_normalization_1 */
    {.id = 6, .type = AI_LAYER_POOL_TYPE, .input_tensors = {1, (const int32_t[1]){7}}, .output_tensors = {1, (const int32_t[1]){8}} }, /* max_pooling1d_1 */
    {.id = 7, .type = AI_LAYER_CONV2D_TYPE, .input_tensors = {1, (const int32_t[1]){8}}, .output_tensors = {1, (const int32_t[1]){9}} }, /* conv_hidden_2_conv2d */
    {.id = 7, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){9}}, .output_tensors = {1, (const int32_t[1]){10}} }, /* conv_hidden_2 */
    {.id = 8, .type = AI_LAYER_BN_TYPE, .input_tensors = {1, (const int32_t[1]){10}}, .output_tensors = {1, (const int32_t[1]){11}} }, /* batch_normalization_2 */
    {.id = 9, .type = AI_LAYER_POOL_TYPE, .input_tensors = {1, (const int32_t[1]){11}}, .output_tensors = {1, (const int32_t[1]){12}} }, /* max_pooling1d_2 */
    {.id = 11, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){12}}, .output_tensors = {1, (const int32_t[1]){13}} }, /* dense_out */
    {.id = 13, .type = AI_LAYER_SM_TYPE, .input_tensors = {1, (const int32_t[1]){13}}, .output_tensors = {1, (const int32_t[1]){14}} } /* output */
  },
  .n_nodes = 14
};
#endif