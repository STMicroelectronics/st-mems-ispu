/**
  *******************************************************************************
  * @file    motion_fa.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @brief   Header for motion_fa module
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
#ifndef MOTION_FA_H
#define MOTION_FA_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_FA MOTION_FA
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
typedef struct
{
  float acc[3];  /* Accelerometer sensor output [g] */
  float gyr[3];  /* Gyroscope sensor output [dps] */
  float prs;     /* Pressure sensor output [hPa] */
  float dtime;   /* Delta-time value [s] */
} MFA_input_t;

typedef struct
{
  uint16_t bicep;     /* Number of bicep curls [#] */
  uint16_t squat;     /* Number of squats [#] */
  uint16_t push_up;   /* Number of push-ups [#] */
  uint16_t lateral;   /* Number of lateral raises [#] */
  uint16_t overhead;  /* Number of overhead presses [#] */
  uint16_t jack;      /* Number of jumping jacks [#] */
} MFA_output_t;

typedef enum
{
  MFA_DISABLE = 0,
  MFA_ENABLE = 1
} MFA_enable_t;

typedef enum
{
  MFA_LEFT = 0,
  MFA_RIGHT
} MFA_wrist_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_FA_Exported_Functions MOTION_FA_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionFA engine.
 * @param  wrist wrist code (left or right wrist)
 * @retval none
 */
void MotionFA_initialize(MFA_wrist_t wrist);

/**
 * @brief  Run fitness activity algorithm.
 * @param  data_in pointer to the structure containing the input data
 * @param  data_out pointer to the structure containing the output data
 * @retval none
 */
void MotionFA_update(MFA_output_t *data_out, MFA_input_t *data_in);

/**
 * @brief  Set sensor orientation, default orientation is ENU (x - east, y - north, z - up).
 * @param  acc_orientation string with reference to set
 * @param  gyr_orientation string with reference to set
 * @retval none
 */
void MotionFA_set_orientation(const char acc_orientation[4], const char gyro_orientation[4]);

/**
 * @brief  Enable or disable and reset the biceps curls counting.
 * @param  enable enable flag (0: disable, 1: enable)
 * @retval none
 */
void MotionFA_bicep_enable(MFA_enable_t enable);

/**
 * @brief  Enable or disable and reset the squats counting.
 * @param  enable enable flag (0: disable, 1: enable)
 * @retval none
 */
void MotionFA_squat_enable(MFA_enable_t enable);

/**
 * @brief  Enable or disable and reset the push-ups counting.
 * @param  enable enable flag (0: disable, 1: enable)
 * @retval none
 */
void MotionFA_push_up_enable(MFA_enable_t enable);

/**
 * @brief  Enable or disable and reset the lateral raises counting.
 * @param  enable enable flag (0: disable, 1: enable)
 * @retval none
 */
void MotionFA_lateral_enable(MFA_enable_t enable);

/**
 * @brief  Enable or disable and reset the overhead presses counting.
 * @param  enable enable flag (0: disable, 1: enable)
 * @retval none
 */
void MotionFA_overhead_enable(MFA_enable_t enable);

/**
 * @brief  Enable or disable and reset the jumping jacks counting.
 * @param  enable enable flag (0: disable, 1: enable)
 * @retval none
 */
void MotionFA_jack_enable(MFA_enable_t enable);

/**
  * @brief  Get the library version.
  * @param  version pointer to an array of 35 char
  * @retval number of characters in the version string
  */
uint8_t MotionFA_get_lib_version(char *version);

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

#endif /* MOTION_FA_H */

