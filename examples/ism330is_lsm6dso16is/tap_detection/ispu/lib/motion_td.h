/**
  *******************************************************************************
  * @file    motion_td.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @brief   Header for motion_td module
  *******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  *******************************************************************************
  */

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef MOTION_TD_H
#define MOTION_TD_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_TD MOTION_TD
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
typedef struct
{
  float acc[3];  /* Accelerometer sensor output [g] */
} MTD_input_t;

typedef enum
{
  MTD_NONE,
  MTD_SINGLE,
  MTD_DOUBLE,
  MTD_TRIPLE,
} MTD_output_t;

typedef struct {
  uint8_t axis;          /* Detection axis [0-2] */
  float peak_ths;        /* Peak detection threshold [g] */
  uint8_t pre_still_st;  /* Starting sample for pre-still [0-13] */
  uint8_t pre_still_n;   /* Number of samples for pre-still [0-14] */
  float pre_still_ths;   /* Pre-still threshold [g] */
  uint8_t invert_t;      /* Max peak inversion time [#samples] */
  uint8_t wait_t;        /* Wait time after peak(s) [#samples] */
  uint8_t still_t;       /* Post-still time [#samples] */
  float post_still_ths;  /* Post-still threshold [g] */
  uint8_t rebound_t;     /* Rebound time [#samples] */
  uint8_t single_en;     /* Single tap event [0: disabled, 1: enabled] */
  uint8_t double_en;     /* Double tap event [0: disabled, 1: enabled] */
  uint8_t triple_en;     /* Triple tap event [0: disabled, 1: enabled] */
  uint16_t latency_t;    /* Latency time to detect consecutive tap events [#samples] */
  uint8_t wait_latency;  /* Enable waiting the latency to determine whether the event
                            is a single, double or triple tap [0: enabled, 1: disabled] */
} MTD_conf_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_TD_Exported_Functions MOTION_TD_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionTD engine.
 * @retval none
 */
void MotionTD_initialize(void);

/**
 * @brief  Get the algorithm configuration.
 * @param  conf pointer to the structure where the current configuration will be written to
 * @retval none
 */
void MotionTD_get_configuration(MTD_conf_t *conf);

/**
 * @brief  Set the algorithm configuration.
 * @param  conf pointer to the structure containing the configuration to be set
 * @retval success flag [0: invalid configuration, 1: valid configuration]
 */
uint8_t MotionTD_set_configuration(MTD_conf_t *conf);

/**
 * @brief  Run tap detection algorithm.
 * @param  data_in pointer to the structure containing the input data
 * @param  data_out pointer to the enum containing the output data
 * @retval none
 */
void MotionTD_update(MTD_output_t *data_out, MTD_input_t *data_in);

/**
  * @brief  Get the library version.
  * @param  version pointer to an array of 35 char
  * @retval number of characters in the version string
  */
uint8_t MotionTD_get_lib_version(char *version);

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

#endif /* MOTION_TD_H */

