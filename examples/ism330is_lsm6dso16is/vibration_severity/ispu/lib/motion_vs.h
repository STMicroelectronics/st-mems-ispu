/**
  *******************************************************************************
  * @file    motion_vs.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @brief   Header for motion_vs module
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
#ifndef MOTION_VS_H
#define MOTION_VS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_VS MOTION_VS
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
enum
{
  MVS_AXIS_X = 0,
  MVS_AXIS_Y = 1,
  MVS_AXIS_Z = 2
};

enum
{
  MVS_BW_2_1000_HZ = 0,
  MVS_BW_10_1000_HZ = 1,
  MVS_BW_2_100_HZ = 2,
  MVS_BW_10_100_HZ = 3
};

typedef struct
{
  uint8_t axis;                /* Axis selection (x, y, or z) */
  uint8_t bw;                  /* Bandpass filter bandwidth (2-1000 Hz, 10-1000 Hz, 2-100 Hz, 10-100 Hz) */
  uint8_t hp_en;               /* Additional hp filter (0: disable, 1: enable) */
  uint16_t period;             /* Period [# samples] */
  uint16_t rms_update_period;  /* Velocity RMS update period [# samples] */
} MVS_conf_t;

typedef struct
{
  float acc[3];  /* Accelerometer sensor output [mm / s^2] */
  float dtime;   /* Delta-time value [s] */
} MVS_input_t;

typedef struct
{
  float acc_filt;          /* Band-pass filtered acceleration [mm / s^2] */
  float vel;               /* Velocity [mm / s] */
  float vel_rms;           /* Velocity RMS [mm / s] */
  uint8_t vel_rms_update;  /* Velocity RMS update flag (1 if updated) */
} MVS_output_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_VS_Exported_Functions MOTION_VS_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionVS engine.
 * @param  conf pointer to the structure containing the configuration to set
 * @retval none
 */
void MotionVS_initialize(MVS_conf_t *conf);

/**
 * @brief  Run vibration severity algorithm.
 * @param  data_in pointer to the structure containing the input data
 * @param  data_out pointer to the structure containing the output data
 * @retval none
 */
void MotionVS_update(MVS_output_t *data_out, MVS_input_t *data_in);

/**
  * @brief  Get the library version.
  * @param  version pointer to an array of 35 char
  * @retval number of characters in the version string
  */
uint8_t MotionVS_get_lib_version(char *version);

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

#endif /* MOTION_VS_H */

