/**
  *******************************************************************************
  * @file    motion_wt.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @brief   Header for motion_wt module
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
#ifndef MOTION_WT_H
#define MOTION_WT_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_WT MOTION_WT
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
typedef struct
{
  float acc[3];  /* Accelerometer sensor output [g] */
} MWT_input_t;

typedef struct
{
  uint8_t wrist_up_en;             /* Wrist up event [0: disabled, 1: enabled] */
  uint8_t wrist_down_en;           /* Wrist down event [0: disabled, 1: enabled] */

  float wrist_up_trigger_ths;      /* Wrist up trigger threshold [g] */
  uint16_t wrist_up_trigger_t;     /* Wrist up trigger timer [# samples] */
  float wrist_up_tilt_x_ths;       /* Wrist up tilt threshold on x-axis [g] */
  float wrist_up_tilt_y_ths;       /* Wrist up tilt threshold on y-axis [g] */
  uint16_t wrist_up_tilt_t;        /* Wrist up tilt timer [# samples] */
  float wrist_up_discharge_ths;    /* Wrist up discharge threshold [g] */

  float wrist_down_trigger_ths;    /* Wrist down trigger threshold [g] */
  uint16_t wrist_down_trigger_t;   /* Wrist down trigger timer [# samples] */
  float wrist_down_tilt_x_ths;     /* Wrist down tilt threshold on x-axis [g] */
  float wrist_down_tilt_y_ths;     /* Wrist down tilt threshold on y-axis [g] */
  uint16_t wrist_down_tilt_t;      /* Wrist down tilt timer [# samples] */
  float wrist_down_discharge_ths;  /* Wrist down discharge threshold [g] */
} MWT_conf_t;

typedef struct
{
  uint8_t wrist_up;    /* Wrist up event detection [0: none, 1: detected]. */
  uint8_t wrist_down;  /* Wrist down event detection [0: none, 1: detected]. */
} MWT_output_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_WT_Exported_Functions MOTION_WT_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionWT engine
 * @retval none
 */
void MotionWT_initialize(void);

/**
 * @brief  Get the algorithm configuration.
 * @param  conf pointer to the structure where the current configuration will be written to
 * @retval none
 */
void MotionWT_get_configuration(MWT_conf_t *conf);

/**
 * @brief  Set the algorithm configuration.
 * @param  conf pointer to the structure containing the configuration to be set
 * @retval none
 */
void MotionWT_set_configuration(MWT_conf_t *conf);

/**
 * @brief  Run wrist-tilt algorithm (to be called at 26 Hz pace).
 * @param  data_out pointer to the structure containing the output data
 * @param  data_in pointer to the structure containing the input data
 * @retval none
 */
void MotionWT_update(MWT_output_t *data_out, MWT_input_t *data_in);

/**
  * @brief  Get the library version
  * @param  version pointer to an array of 35 char
  * @retval number of characters in the version string
  */
uint8_t MotionWT_get_lib_version(char *version);

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

#endif /* _MOTION_WT_H_ */

