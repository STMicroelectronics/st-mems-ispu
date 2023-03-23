/**
  *******************************************************************************
  * @file    motion_mc.h
  * @author  MEMS Application Team
  * @version V1.0.0
  * @brief   Header for motion_mc module
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
#ifndef MOTION_MC_H
#define MOTION_MC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_MC MOTION_MC
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
typedef enum
{
  MMC_HI_ONLY = 0,
  MMC_HI_AND_SI = 1
} MMC_mode_t;

typedef enum
{
  MMC_CALQSTATUSUNCALIBRATED = 0,
  MMC_CALQSTATUSPOOR = 1,
  MMC_CALQSTATUSGOOD = 2,
  MMC_CALQSTATUSBEST = 3
} MMC_cal_quality_t;

typedef struct
{
  float dtime;   /* Delta time since previous call [s] */
  float mag[3];  /* Magnetometer sensor output [uT/50] */
} MMC_input_t;

typedef struct
{
  float HI_bias[3];               /* Hard iron offset array [uT/50] */
  float SI_matrix[3][3];          /* Soft iron correction matrix [-] */
  float B;                        /* Estimated magnetic field [uT/50]
                                   (to be used in HI only mode) */
  MMC_cal_quality_t cal_quality;  /* Calibration quality factor */
} MMC_output_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_MC_Exported_Functions MOTION_MC_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionMC engine
 * @param  mode calibration mode to set, hard-iron only mode (faster) or hard-iron + soft-iron
 * @retval pointer to the new algorithm instance
 */
void *MotionMC_initialize(MMC_mode_t mode);

/**
 * @brief  Deinitialize the MotionMC engine
 * @param  mmc pointer to the algorithm instance
 * @retval none
 */
void MotionMC_deinitialize(void *mmc);

/**
 * @brief  This function runs one step of the magnetometer calibration algorithm
 * @param  mmc pointer to the algorithm instance
 * @param  data_out pointer to the structure containing the output data
 * @param  data_in pointer to the structure containing the input data
 * @retval none
 */
void MotionMC_update(void *mmc, MMC_output_t *data_out, MMC_input_t *data_in);

/**
 * @brief  Get the library version
 * @param  version pointer to an array of 35 char
 * @retval number of characters in the version string
 */
uint8_t MotionMC_get_lib_version(char *version);

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

#endif /* _MOTION_MC_H_ */

