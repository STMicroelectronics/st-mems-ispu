/**
  *******************************************************************************
  * @file    motion_ft.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @brief   Header for motion_ft module
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *******************************************************************************
  */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef MOTION_FT_H
#define MOTION_FT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_FT MOTION_FT
 *  @{
 */

/* Exported types ----------------------------------------------------------- */

typedef struct
{
  uint16_t winlen;    /* Length of sliding window [#samples] */
  float damping;      /* Damping factor to ensure stability [0 ≤ damping ≤ 1] */
  uint8_t normalize;  /* Apply scaling factor of 1 / winlen [0: disabled, 1: enabled] */
} MFT_conf_t;

typedef struct
{
  float sample;  /* Signal sample */
} MFT_input_t;

typedef struct
{
  float *dft_mag;  /* DFT magnitudes (array of (winlen / 2) + 1 length to be allocated by the user) */
} MFT_output_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_FT_Exported_Functions MOTION_FT_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionFT engine.
 * @param  conf pointer to the structure containing the configuration to set
 * @retval pointer to the new algorithm instance
 */
void *MotionFT_initialize(MFT_conf_t *conf);

/**
 * @brief  Deinitialize the MotionFT engine.
 * @param  mft pointer to the algorithm instance
 * @retval none
 */
void MotionFT_deinitialize(void *mft);

/**
 * @brief  Run one step of the sliding DFT algorithm.
 * @param  mft pointer to the algorithm instance
 * @param  data_out pointer to the structure containing the output data
 * @param  data_in pointer to the structure containing the input data
 * @retval none
 */
void MotionFT_update(void *mft, MFT_output_t *data_out, MFT_input_t *data_in);

/**
 * @brief  Get the library version.
 * @param  version pointer to an array of 35 char
 * @retval number of characters in the version string
 */
uint8_t MotionFT_get_lib_version(char *version);

/**
 *  @}
 */

/**
 *  @}
 */

/**
 *  @}
 */

#ifdef __cplusplus
}
#endif

#endif /* MOTION_FT_H */

