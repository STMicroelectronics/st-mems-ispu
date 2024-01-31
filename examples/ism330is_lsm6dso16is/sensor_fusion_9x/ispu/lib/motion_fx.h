/**
  *******************************************************************************
  * @file    motion_fx.h
  * @author  MEMS Application Team
  * @version V1.0.1
  * @brief   Header for motion_fx module
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
#ifndef MOTION_FX_H
#define MOTION_FX_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ----------------------------------------------------------------- */
#include <stdint.h>

/** @addtogroup MIDDLEWARES
 *  @{
 */

/** @defgroup MOTION_FX MOTION_FX
 *  @{
 */

/* Exported types ----------------------------------------------------------- */
typedef enum
{
  MFX_ENGINE_DISABLE = 0,
  MFX_ENGINE_ENABLE = 1
} MFX_engine_state_t;

typedef enum
{
  MFX_6X = 0,
  MFX_9X = 1
} MFX_mode_t;

typedef struct
{
  float mag[3];   /* Calibrated mag [uT/50] */
  float acc[3];   /* Acceleration in [g] */
  float gyro[3];  /* Angular rate [dps] */
} MFX_input_t;

typedef struct
{
  float quaternion[4];           /* Quaternion [-] */
  float gravity[3];              /* Device frame gravity [g] */
  float linear_acceleration[3];  /* Device frame linear acceleration [g] */
  float rotation[3];             /* Yaw, pitch and roll [deg] */
} MFX_output_t;

typedef struct
{
  float alpha_acc;              /* Coefficient to control accelerometer mean and variance computation [0 to 1] */
  float alpha_gyr;              /* Coefficient to control gyroscope mean and variance computation [0 to 1] */
  float alpha_mag;              /* Coefficient to control magnetometer mean and variance computation [0 to 1] */
  float kappa_acc;              /* Coefficient to control accelerometer weighting factor [0 to 1] */
  float kappa_mag;              /* Coefficient to control magnetometer weighting factor [0 to 1] */
  float acc_max;                /* Linear acceleration threshold [g] */
  float acc_var_max;            /* Linear acceleration var-based threshold [g] */
  float mag_var_max;            /* Magnetic anomaly var-based threshold [g] */
  float gb_alpha;               /* Gbias calibration low-pass filter [0 to 1] */
  float gb_acc_max;             /* Gbias calibration acceleration norm-based threshold [g] */
  float gb_gyr_max;             /* Gbias calibration angular rate threshold [dps] */
  float gb_acc_var_max;         /* Gbias calibration acceleration var-based threshold [g] */
  float gb_gyr_var_max;         /* Gbias calibration angular rate var-based threshold [dps] */
  float gb_gyr_var_max_greedy;  /* Gbias calibration initial angular rate var-based threshold [dps] */
  float gb_time;                /* Gbias calibration time-based threshold [s] */
} MFX_conf_t;

/**
 *  @}
 */

/* Exported constants ------------------------------------------------------- */
/* Exported variables ------------------------------------------------------- */
/* Exported macro ----------------------------------------------------------- */

/** @defgroup MOTION_FX_Exported_Functions MOTION_FX_Exported_Functions
 *  @{
 */

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initialize the MotionFX engine.
 * @param  mode algorithm mode to set
 * @retval pointer to the new algorithm instance
 */
void *MotionFX_initialize(MFX_mode_t mode);

/**
 * @brief  Deinitialize the MotionFX engine.
 * @param  mfx pointer to the algorithm instance
 * @retval none
 */
void MotionFX_deinitialize(void *mfx);

/**
 * @brief  Get the algorithm configuration.
 * @param  mfx pointer to the algorithm instance
 * @param  conf pointer to the structure where the current configuration will be written to
 * @retval none
 */
void MotionFX_get_configuration(void *mfx, MFX_conf_t *conf);

/**
 * @brief  Set the algorithm configuration.
 * @param  mfx pointer to the algorithm instance
 * @param  conf pointer to the structure containing the configuration to be set
 * @retval none
 */
void MotionFX_set_configuration(void *mfx, MFX_conf_t *conf);

/**
 * @brief  Set sensor orientation, default orientation is ENU (x - east, y - north, z - up).
 * @param  mfx pointer to the algorithm instance
 * @param  acc_orientation string with reference to set
 * @param  gyr_orientation string with reference to set
 * @param  mag_orientation string with reference to set
 * @retval none
 */
void MotionFX_set_orientation(void *mfx, const char acc_orientation[4], const char gyro_orientation[4], const char mag_orientation[4]);

/**
 * @brief  Get the status of the euler angles calculation.
 * @param  mfx pointer to the algorithm instance
 * @retval 1 if enabled, 0 if disabled
 */
MFX_engine_state_t MotionFX_get_status_euler(void *mfx);

/**
 * @brief  Get the status of the gyroscope calibration.
 * @param  mfx pointer to the algorithm instance
 * @retval 1 if enabled, 0 if disabled
 */
MFX_engine_state_t MotionFX_get_status_gbias(void *mfx);

/**
 * @brief  Enable or disable euler angles calculation.
 * @param  mfx pointer to the algorithm instance
 * @param  1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_enable_euler(void *mfx, MFX_engine_state_t enable);

/**
 * @brief  Enable or disable gyroscope calibration.
 * @param  mfx pointer to the algorithm instance
 * @param  1 to enable, 0 to disable
 * @retval none
 */
void MotionFX_enable_gbias(void *mfx, MFX_engine_state_t enable);

/**
 * @brief  Set the initial gbias.
 * @param  mfx pointer to the algorithm instance
 * @param  gbias pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_set_gbias(void *mfx, float *gbias);

/**
 * @brief  Get the initial gbias.
 * @param  mfx pointer to the algorithm instance
 * @param  gbias pointer to a float array containing the 3 gbias values
 * @retval none
 */
void MotionFX_get_gbias(void *mfx, float *gbias);

/**
 * @brief  Run one step of the sensor fusion algorithm.
 * @param  mfx pointer to the algorithm instance
 * @param  data_out pointer to the structure containing the output data
 * @param  data_in pointer to the structure containing the input data
 * @param  dtime delta time between two calls [s]
 * @retval none
 */
void MotionFX_update(void *mfx, MFX_output_t *data_out, MFX_input_t *data_in, float dtime);

/**
 * @brief  Get the library version.
 * @param  version pointer to an array of 35 char
 * @retval number of characters in the version string
 */
uint8_t MotionFX_get_lib_version(char *version);

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

#endif /* MOTION_FX_H */

