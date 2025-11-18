/**
  ******************************************************************************
  * @file    motion_aw.h
  * @author  MEMS Application Team
  * @version V2.1.0
  * @date    01-May-2017
  * @brief   Header for motion_aw module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MOTION_AW_H
#define MOTION_AW_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup MIDDLEWARES
  * @{
  */

/** @defgroup MOTION_AW MOTION_AW
  * @{
  */

/** @defgroup MOTION_AW_Exported_Types MOTION_AW_Exported_Types
 * @{
 */

/* Exported constants --------------------------------------------------------*/
#define NUM_ACTIVITIES 10

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  float AccX;           /* Acceleration in X axis in [g] */
  float AccY;           /* Acceleration in Y axis in [g] */
  float AccZ;           /* Acceleration in Z axis in [g] */
} MAW_input_t;

typedef enum
{
  MAW_NOACTIVITY     = 0x00,
  MAW_STATIONARY     = 0x01,
  MAW_STANDING       = 0x02,
  MAW_SITTING        = 0x03,
  MAW_LYING          = 0x04,
  MAW_WALKING        = 0x05,
  MAW_FASTWALKING    = 0x06,
  MAW_JOGGING        = 0x07,
  MAW_BIKING         = 0x08,
  MAW_OTHERS         = 0x09
} MAW_activity_t;

/* Confidence Level : Range 0-5 [Low -high].
 * current_activity_duration : the duration of current acitivty
 * activity_total_duration will report duration of each activity */
typedef struct {
  MAW_activity_t current_activity;
  uint16_t confidence;
  uint32_t current_activity_duration;
  uint32_t activity_total_duration[NUM_ACTIVITIES];
} MAW_output_t;

/* Structure to hold the meta classifier counter.
 * WinSize : The window used for performing operation of meta classification. Maximum value 32
 * minVote : minimum number of detection in window to declare as class. MinVote can not be more than winSize */
typedef struct {
	uint8_t WinSize[NUM_ACTIVITIES];  // max 32
	uint8_t MinVote[NUM_ACTIVITIES];  // max 32
} MAW_metaClassifier_t;

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup MOTION_AW_Exported_Functions MOTION_AW_Exported_Functions
 * @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionAW engine.
 * @param  none
 * @retval none
 */
void MotionAW_Initialize(void);

/**
 * @brief  Set the MotionAW accelerometer data orientation.
 * @param  acc_orientation reference system of the accelerometer raw data (for instance: south west up became "swu", north east up became "ned")
 * @retval none
 */
void MotionAW_SetOrientation_Acc(const char *acc_orientation);

/**
 * @brief  Run activity recognition algorithm.
 * @param  data_in pointer to acceleration in [g]
 * @param  t timestamp in [ms]
 * @param  data_out pointer to output activity structure
 * @retval none
 */
void MotionAW_Update(MAW_input_t *data_in, MAW_output_t *data_out, int64_t t);

/**
 * @brief  Resets activity recognition algorithm.
 * @param  none
 * @retval none
 */
void MotionAW_Reset(void);

/**
 * @brief  Resets total activity duration counters.
 * @param  none
 * @retval none
 */
void MotionAW_Reset_Activity_Duration(void);

/**
 * @brief  Get the library version.
 * @param  version pointer to an array of 35 char
 * @retval Number of characters in the version string
 */
uint8_t MotionAW_GetLibVersion(char *version);

/**
 * @brief  Get Meta Classifier Counter.
 * @param  MAW_metaClassifier_t pointer
 * @retval none
 */
void MotionAW_GetMetaClassifier(MAW_metaClassifier_t *metaCounter);

/**
 * @brief  Set Meta Classifier Counter: API should be called just after initialization but before using Update function.
 * @param  MAW_metaClassifier_t pointer
 * @retval none
 */
void MotionAW_SetMetaClassifier(MAW_metaClassifier_t *metaCounter);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* MOTION_AW_H */

