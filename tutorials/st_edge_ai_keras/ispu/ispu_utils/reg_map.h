/**
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

#ifndef REG_MAP_H
#define REG_MAP_H

#define ctrl_reg_base 0x6800

#define ISPU_GLB_CALL_EN  (ctrl_reg_base + 0x00)
#define ISPU_CONFIG       (ctrl_reg_base + 0x02)
#define ISPU_STATUS       (ctrl_reg_base + 0x04)
#define ISPU_IF2S_FLAG_L  (ctrl_reg_base + 0x0C)
#define ISPU_IF2S_FLAG_H  (ctrl_reg_base + 0x0D)
#define ISPU_IF2S_FLAG    (ISPU_IF2S_FLAG_L)
#define ISPU_S2IF_FLAG_L  (ctrl_reg_base + 0x0E)
#define ISPU_S2IF_FLAG_H  (ctrl_reg_base + 0x0F)
#define ISPU_S2IF_FLAG    (ISPU_S2IF_FLAG_L)
#define ISPU_DOUT_00_L    (ctrl_reg_base + 0x10)
#define ISPU_DOUT_00_H    (ctrl_reg_base + 0x11)
#define ISPU_DOUT_00      (ISPU_DOUT_00_L)
#define ISPU_DOUT_01_L    (ctrl_reg_base + 0x12)
#define ISPU_DOUT_01_H    (ctrl_reg_base + 0x13)
#define ISPU_DOUT_01      (ISPU_DOUT_01_L)
#define ISPU_DOUT_02_L    (ctrl_reg_base + 0x14)
#define ISPU_DOUT_02_H    (ctrl_reg_base + 0x15)
#define ISPU_DOUT_02      (ISPU_DOUT_02_L)
#define ISPU_DOUT_03_L    (ctrl_reg_base + 0x16)
#define ISPU_DOUT_03_H    (ctrl_reg_base + 0x17)
#define ISPU_DOUT_03      (ISPU_DOUT_03_L)
#define ISPU_DOUT_04_L    (ctrl_reg_base + 0x18)
#define ISPU_DOUT_04_H    (ctrl_reg_base + 0x19)
#define ISPU_DOUT_04      (ISPU_DOUT_04_L)
#define ISPU_DOUT_05_L    (ctrl_reg_base + 0x1A)
#define ISPU_DOUT_05_H    (ctrl_reg_base + 0x1B)
#define ISPU_DOUT_05      (ISPU_DOUT_05_L)
#define ISPU_DOUT_06_L    (ctrl_reg_base + 0x1C)
#define ISPU_DOUT_06_H    (ctrl_reg_base + 0x1D)
#define ISPU_DOUT_06      (ISPU_DOUT_06_L)
#define ISPU_DOUT_07_L    (ctrl_reg_base + 0x1E)
#define ISPU_DOUT_07_H    (ctrl_reg_base + 0x1F)
#define ISPU_DOUT_07      (ISPU_DOUT_07_L)
#define ISPU_DOUT_08_L    (ctrl_reg_base + 0x20)
#define ISPU_DOUT_08_H    (ctrl_reg_base + 0x21)
#define ISPU_DOUT_08      (ISPU_DOUT_08_L)
#define ISPU_DOUT_09_L    (ctrl_reg_base + 0x22)
#define ISPU_DOUT_09_H    (ctrl_reg_base + 0x23)
#define ISPU_DOUT_09      (ISPU_DOUT_09_L)
#define ISPU_DOUT_10_L    (ctrl_reg_base + 0x24)
#define ISPU_DOUT_10_H    (ctrl_reg_base + 0x25)
#define ISPU_DOUT_10      (ISPU_DOUT_10_L)
#define ISPU_DOUT_11_L    (ctrl_reg_base + 0x26)
#define ISPU_DOUT_11_H    (ctrl_reg_base + 0x27)
#define ISPU_DOUT_11      (ISPU_DOUT_11_L)
#define ISPU_DOUT_12_L    (ctrl_reg_base + 0x28)
#define ISPU_DOUT_12_H    (ctrl_reg_base + 0x29)
#define ISPU_DOUT_12      (ISPU_DOUT_12_L)
#define ISPU_DOUT_13_L    (ctrl_reg_base + 0x2A)
#define ISPU_DOUT_13_H    (ctrl_reg_base + 0x2B)
#define ISPU_DOUT_13      (ISPU_DOUT_13_L)
#define ISPU_DOUT_14_L    (ctrl_reg_base + 0x2C)
#define ISPU_DOUT_14_H    (ctrl_reg_base + 0x2D)
#define ISPU_DOUT_14      (ISPU_DOUT_14_L)
#define ISPU_DOUT_15_L    (ctrl_reg_base + 0x2E)
#define ISPU_DOUT_15_H    (ctrl_reg_base + 0x2F)
#define ISPU_DOUT_15      (ISPU_DOUT_15_L)
#define ISPU_DOUT_16_L    (ctrl_reg_base + 0x30)
#define ISPU_DOUT_16_H    (ctrl_reg_base + 0x31)
#define ISPU_DOUT_16      (ISPU_DOUT_16_L)
#define ISPU_DOUT_17_L    (ctrl_reg_base + 0x32)
#define ISPU_DOUT_17_H    (ctrl_reg_base + 0x33)
#define ISPU_DOUT_17      (ISPU_DOUT_17_L)
#define ISPU_DOUT_18_L    (ctrl_reg_base + 0x34)
#define ISPU_DOUT_18_H    (ctrl_reg_base + 0x35)
#define ISPU_DOUT_18      (ISPU_DOUT_18_L)
#define ISPU_DOUT_19_L    (ctrl_reg_base + 0x36)
#define ISPU_DOUT_19_H    (ctrl_reg_base + 0x37)
#define ISPU_DOUT_19      (ISPU_DOUT_19_L)
#define ISPU_DOUT_20_L    (ctrl_reg_base + 0x38)
#define ISPU_DOUT_20_H    (ctrl_reg_base + 0x39)
#define ISPU_DOUT_20      (ISPU_DOUT_20_L)
#define ISPU_DOUT_21_L    (ctrl_reg_base + 0x3A)
#define ISPU_DOUT_21_H    (ctrl_reg_base + 0x3B)
#define ISPU_DOUT_21      (ISPU_DOUT_21_L)
#define ISPU_DOUT_22_L    (ctrl_reg_base + 0x3C)
#define ISPU_DOUT_22_H    (ctrl_reg_base + 0x3D)
#define ISPU_DOUT_22      (ISPU_DOUT_22_L)
#define ISPU_DOUT_23_L    (ctrl_reg_base + 0x3E)
#define ISPU_DOUT_23_H    (ctrl_reg_base + 0x3F)
#define ISPU_DOUT_23      (ISPU_DOUT_23_L)
#define ISPU_DOUT_24_L    (ctrl_reg_base + 0x40)
#define ISPU_DOUT_24_H    (ctrl_reg_base + 0x41)
#define ISPU_DOUT_24      (ISPU_DOUT_24_L)
#define ISPU_DOUT_25_L    (ctrl_reg_base + 0x42)
#define ISPU_DOUT_25_H    (ctrl_reg_base + 0x43)
#define ISPU_DOUT_25      (ISPU_DOUT_25_L)
#define ISPU_DOUT_26_L    (ctrl_reg_base + 0x44)
#define ISPU_DOUT_26_H    (ctrl_reg_base + 0x45)
#define ISPU_DOUT_26      (ISPU_DOUT_26_L)
#define ISPU_DOUT_27_L    (ctrl_reg_base + 0x46)
#define ISPU_DOUT_27_H    (ctrl_reg_base + 0x47)
#define ISPU_DOUT_27      (ISPU_DOUT_27_L)
#define ISPU_DOUT_28_L    (ctrl_reg_base + 0x48)
#define ISPU_DOUT_28_H    (ctrl_reg_base + 0x49)
#define ISPU_DOUT_28      (ISPU_DOUT_28_L)
#define ISPU_DOUT_29_L    (ctrl_reg_base + 0x4A)
#define ISPU_DOUT_29_H    (ctrl_reg_base + 0x4B)
#define ISPU_DOUT_29      (ISPU_DOUT_29_L)
#define ISPU_DOUT_30_L    (ctrl_reg_base + 0x4C)
#define ISPU_DOUT_30_H    (ctrl_reg_base + 0x4D)
#define ISPU_DOUT_30      (ISPU_DOUT_30_L)
#define ISPU_DOUT_31_L    (ctrl_reg_base + 0x4E)
#define ISPU_DOUT_31_H    (ctrl_reg_base + 0x4F)
#define ISPU_DOUT_31      (ISPU_DOUT_31_L)
#define ISPU_INT1_CTRL0   (ctrl_reg_base + 0x50)
#define ISPU_INT1_CTRL1   (ctrl_reg_base + 0x51)
#define ISPU_INT1_CTRL2   (ctrl_reg_base + 0x52)
#define ISPU_INT1_CTRL3   (ctrl_reg_base + 0x53)
#define ISPU_INT1_CTRL    (ISPU_INT1_CTRL0)
#define ISPU_INT2_CTRL0   (ctrl_reg_base + 0x54)
#define ISPU_INT2_CTRL1   (ctrl_reg_base + 0x55)
#define ISPU_INT2_CTRL2   (ctrl_reg_base + 0x56)
#define ISPU_INT2_CTRL3   (ctrl_reg_base + 0x57)
#define ISPU_INT2_CTRL    (ISPU_INT2_CTRL0)
#define ISPU_INT_STATUS0  (ctrl_reg_base + 0x58)
#define ISPU_INT_STATUS1  (ctrl_reg_base + 0x59)
#define ISPU_INT_STATUS2  (ctrl_reg_base + 0x5A)
#define ISPU_INT_STATUS3  (ctrl_reg_base + 0x5B)
#define ISPU_INT_STATUS   (ISPU_INT_STATUS0)
#define ISPU_INT_PIN      (ctrl_reg_base + 0x5C)
#define ISPU_ALGO0        (ctrl_reg_base + 0x70)
#define ISPU_ALGO1        (ctrl_reg_base + 0x71)
#define ISPU_ALGO2        (ctrl_reg_base + 0x72)
#define ISPU_ALGO3        (ctrl_reg_base + 0x73)
#define ISPU_ALGO         (ISPU_ALGO0)
#define ISPU_ARAW_X_L     (ctrl_reg_base + 0x80)
#define ISPU_ARAW_X_H     (ctrl_reg_base + 0x81)
#define ISPU_ARAW_X_EL    (ctrl_reg_base + 0x82) // sign extention
#define ISPU_ARAW_X_EH    (ctrl_reg_base + 0x83) // sign extension
#define ISPU_ARAW_X       (ISPU_ARAW_X_L)
#define ISPU_ARAW_Y_L     (ctrl_reg_base + 0x84)
#define ISPU_ARAW_Y_H     (ctrl_reg_base + 0x85)
#define ISPU_ARAW_Y_EL    (ctrl_reg_base + 0x86) // sign extension
#define ISPU_ARAW_Y_EH    (ctrl_reg_base + 0x87) // sign extension
#define ISPU_ARAW_Y       (ISPU_ARAW_Y_L)
#define ISPU_ARAW_Z_L     (ctrl_reg_base + 0x88)
#define ISPU_ARAW_Z_H     (ctrl_reg_base + 0x89)
#define ISPU_ARAW_Z_EL    (ctrl_reg_base + 0x8A) // sign extension
#define ISPU_ARAW_Z_EH    (ctrl_reg_base + 0x8B) // sign extension
#define ISPU_ARAW_Z       (ISPU_ARAW_Z_L)
#define ISPU_GRAW_X_L     (ctrl_reg_base + 0x8C)
#define ISPU_GRAW_X_H     (ctrl_reg_base + 0x8D)
#define ISPU_GRAW_X_EL    (ctrl_reg_base + 0x8E) // sign extension
#define ISPU_GRAW_X_EH    (ctrl_reg_base + 0x8F) // sign extention
#define ISPU_GRAW_X       (ISPU_GRAW_X_L)
#define ISPU_GRAW_Y_L     (ctrl_reg_base + 0x90)
#define ISPU_GRAW_Y_H     (ctrl_reg_base + 0x91)
#define ISPU_GRAW_Y_EL    (ctrl_reg_base + 0x92) // sign extension
#define ISPU_GRAW_Y_EH    (ctrl_reg_base + 0x93) // sign extension
#define ISPU_GRAW_Y       (ISPU_GRAW_Y_L)
#define ISPU_GRAW_Z_L     (ctrl_reg_base + 0x94)
#define ISPU_GRAW_Z_H     (ctrl_reg_base + 0x95)
#define ISPU_GRAW_Z_EL    (ctrl_reg_base + 0x96) // sign extension
#define ISPU_GRAW_Z_EH    (ctrl_reg_base + 0x97) // sign extension
#define ISPU_GRAW_Z       (ISPU_GRAW_Z_L)
#define ISPU_ERAW_0_L     (ctrl_reg_base + 0x98)
#define ISPU_ERAW_0_H     (ctrl_reg_base + 0x99)
#define ISPU_ERAW_0_EL    (ctrl_reg_base + 0x9A) // sign extension
#define ISPU_ERAW_0_EH    (ctrl_reg_base + 0x9B) // sign extension
#define ISPU_ERAW_0       (ISPU_ERAW_0_L)
#define ISPU_ERAW_1_L     (ctrl_reg_base + 0x9C)
#define ISPU_ERAW_1_H     (ctrl_reg_base + 0x9D)
#define ISPU_ERAW_1_EL    (ctrl_reg_base + 0x9E) // sign extension
#define ISPU_ERAW_1_EH    (ctrl_reg_base + 0x9F) // sign extension
#define ISPU_ERAW_1       (ISPU_ERAW_1_L)
#define ISPU_ERAW_2_L     (ctrl_reg_base + 0xA0)
#define ISPU_ERAW_2_H     (ctrl_reg_base + 0xA1)
#define ISPU_ERAW_2_EL    (ctrl_reg_base + 0xA2) // sign extension
#define ISPU_ERAW_2_EH    (ctrl_reg_base + 0xA3) // sign extension
#define ISPU_ERAW_2       (ISPU_ERAW_2_L)
#define ISPU_TEMP_L       (ctrl_reg_base + 0xA4)
#define ISPU_TEMP_H       (ctrl_reg_base + 0xA5)
#define ISPU_TEMP_EL      (ctrl_reg_base + 0xA6) // sign extension
#define ISPU_TEMP_EH      (ctrl_reg_base + 0xA7) // sign extension
#define ISPU_TEMP         (ISPU_TEMP_L)
#define ISPU_CALL_EN_0    (ctrl_reg_base + 0xB8)
#define ISPU_CALL_EN_1    (ctrl_reg_base + 0xB9)
#define ISPU_CALL_EN_2    (ctrl_reg_base + 0xBA)
#define ISPU_CALL_EN_3    (ctrl_reg_base + 0xBB)
#define ISPU_CALL_EN      (ISPU_CALL_EN_0)

#define ctrl_reg_ext 0x6900

#define TIMESTAMP0        (ctrl_reg_ext + 0x40)
#define TIMESTAMP2        (ctrl_reg_ext + 0x42)
#define ISPU_DTIME_0      (ctrl_reg_ext + 0x48)
#define ISPU_DTIME_1      (ctrl_reg_ext + 0x4A)
#define ISPU_DUMMY_CFG_1  (ctrl_reg_ext + 0x74)
#define ISPU_DUMMY_CFG_2  (ctrl_reg_ext + 0x76)
#define ISPU_DUMMY_CFG_3  (ctrl_reg_ext + 0x78)
#define ISPU_DUMMY_CFG_4  (ctrl_reg_ext + 0x7A)

#endif

