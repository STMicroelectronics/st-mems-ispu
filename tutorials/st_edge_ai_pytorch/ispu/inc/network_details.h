/**
  ******************************************************************************
  * @file    network.h
  * @date    2025-03-05T14:42:09+0100
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
  .tensors = (const stai_tensor[7]) {
   { .size_bytes = 36, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {2, (const int32_t[2]){1, 9}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "onnxGemm_0_output" },
   { .size_bytes = 9, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 9}}, .scale = {1, (const float[1]){0.060613542795181274}}, .zeropoint = {1, (const int16_t[1]){-40}}, .name = "onnxGemm_0_QuantizeLinear_Output_output" },
   { .size_bytes = 64, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 64}}, .scale = {1, (const float[1]){0.08429329097270966}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_1_Relu_output_0_output" },
   { .size_bytes = 32, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 32}}, .scale = {1, (const float[1]){0.8229387402534485}}, .zeropoint = {1, (const int16_t[1]){-128}}, .name = "_4_Relu_output_0_output" },
   { .size_bytes = 4, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_S8, .shape = {2, (const int32_t[2]){1, 4}}, .scale = {1, (const float[1]){6.865355968475342}}, .zeropoint = {1, (const int16_t[1]){31}}, .name = "_6_Gemm_output_0_output" },
   { .size_bytes = 16, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {2, (const int32_t[2]){1, 4}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "_6_Gemm_output_0_0_conversion_output" },
   { .size_bytes = 16, .flags = (STAI_FLAG_HAS_BATCH|STAI_FLAG_CHANNEL_LAST), .format = STAI_FORMAT_FLOAT32, .shape = {2, (const int32_t[2]){1, 4}}, .scale = {0, NULL}, .zeropoint = {0, NULL}, .name = "node_12_output" }
  },
  .nodes = (const stai_node_details[6]){
    {.id = 7, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){0}}, .output_tensors = {1, (const int32_t[1]){1}} }, /* onnxGemm_0_QuantizeLinear_Output */
    {.id = 9, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){1}}, .output_tensors = {1, (const int32_t[1]){2}} }, /* _1_Relu_output_0 */
    {.id = 12, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){2}}, .output_tensors = {1, (const int32_t[1]){3}} }, /* _4_Relu_output_0 */
    {.id = 15, .type = AI_LAYER_DENSE_TYPE, .input_tensors = {1, (const int32_t[1]){3}}, .output_tensors = {1, (const int32_t[1]){4}} }, /* _6_Gemm_output_0 */
    {.id = 15, .type = AI_LAYER_NL_TYPE, .input_tensors = {1, (const int32_t[1]){4}}, .output_tensors = {1, (const int32_t[1]){5}} }, /* _6_Gemm_output_0_0_conversion */
    {.id = 18, .type = AI_LAYER_SM_TYPE, .input_tensors = {1, (const int32_t[1]){5}}, .output_tensors = {1, (const int32_t[1]){6}} } /* node_12 */
  },
  .n_nodes = 6
};
#endif