/**
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

#define ispu_reg_base 0x80000

#define ISPU_LOOP_START      (ispu_reg_base + 0x80)
#define ISPU_LOOP_END        (ispu_reg_base + 0x84)
#define ISPU_LOOP_CNT        (ispu_reg_base + 0x88)
#define ISPU_FFT_MATCH_TW    (ispu_reg_base + 0x8C)
#define ISPU_FFT_START_TW    (ispu_reg_base + 0x90)
#define ISPU_FFT_END_TW      (ispu_reg_base + 0x94)
#define ISPU_FFT_CURR_TW1    (ispu_reg_base + 0x98)
#define ISPU_FFT_CURR_TW2    (ispu_reg_base + 0x9C)
#define ISPU_FFT_STRIDE_TW   (ispu_reg_base + 0xA0)
#define ISPU_FFT_STATE_TW    (ispu_reg_base + 0xA4)
#define ISPU_FFT_MATCH_D     (ispu_reg_base + 0xC0)
#define ISPU_FFT_CURR_D1     (ispu_reg_base + 0xC4)
#define ISPU_FFT_CURR_D2     (ispu_reg_base + 0xC8)
#define ISPU_FFT_TWIDDLE     (ispu_reg_base + 0xCC)
#define ISPU_FFT_STATE_D     (ispu_reg_base + 0xD0)
#define ISPU_FFT_STATE_BRA   (ispu_reg_base + 0xD4)
#define ISPU_FFT_MATCH_BRA   (ispu_reg_base + 0xD8)
#define ISPU_FFT_CURR_BRA    (ispu_reg_base + 0xDC)
#define ISPU_FFT_N_ELEM_BRA  (ispu_reg_base + 0xE0)
#define ISPU_CIRCULAR_MATCH  (ispu_reg_base + 0xE4)
#define ISPU_CIRCULAR_START  (ispu_reg_base + 0xE8)
#define ISPU_CIRCULAR_END    (ispu_reg_base + 0xEC)
#define ISPU_CIRCULAR_CURR   (ispu_reg_base + 0xF0)
#define ISPU_CIRCULAR_STRIDE (ispu_reg_base + 0xF4)
#define ISPU_SQRT_I          (ispu_reg_base + 0xF8)
#define ISPU_SQRT_O          (ispu_reg_base + 0xFC)

#define ctrl_reg_base 0x80800

#define ISPU_GLB_CALL_EN     (ctrl_reg_base + 0x00)
#define ISPU_CONFIG          (ctrl_reg_base + 0x02)
#define ISPU_STATUS          (ctrl_reg_base + 0x04)

#define ISPU_IF2S_FLAG_L     (ctrl_reg_base + 0x0C)
#define ISPU_IF2S_FLAG_H     (ctrl_reg_base + 0x0D)
#define ISPU_IF2S_FLAG       (ISPU_IF2S_FLAG_L)
#define ISPU_S2IF_FLAG_L     (ctrl_reg_base + 0x0E)
#define ISPU_S2IF_FLAG_H     (ctrl_reg_base + 0x0F)
#define ISPU_S2IF_FLAG       (ISPU_S2IF_FLAG_L)

#define ISPU_DOUT_00_L       (ctrl_reg_base + 0x10)
#define ISPU_DOUT_00_H       (ctrl_reg_base + 0x11)
#define ISPU_DOUT_00         (ISPU_DOUT_00_L)
#define ISPU_DOUT_01_L       (ctrl_reg_base + 0x12)
#define ISPU_DOUT_01_H       (ctrl_reg_base + 0x13)
#define ISPU_DOUT_01         (ISPU_DOUT_01_L)
#define ISPU_DOUT_02_L       (ctrl_reg_base + 0x14)
#define ISPU_DOUT_02_H       (ctrl_reg_base + 0x15)
#define ISPU_DOUT_02         (ISPU_DOUT_02_L)
#define ISPU_DOUT_03_L       (ctrl_reg_base + 0x16)
#define ISPU_DOUT_03_H       (ctrl_reg_base + 0x17)
#define ISPU_DOUT_03         (ISPU_DOUT_03_L)
#define ISPU_DOUT_04_L       (ctrl_reg_base + 0x18)
#define ISPU_DOUT_04_H       (ctrl_reg_base + 0x19)
#define ISPU_DOUT_04         (ISPU_DOUT_04_L)
#define ISPU_DOUT_05_L       (ctrl_reg_base + 0x1A)
#define ISPU_DOUT_05_H       (ctrl_reg_base + 0x1B)
#define ISPU_DOUT_05         (ISPU_DOUT_05_L)
#define ISPU_DOUT_06_L       (ctrl_reg_base + 0x1C)
#define ISPU_DOUT_06_H       (ctrl_reg_base + 0x1D)
#define ISPU_DOUT_06         (ISPU_DOUT_06_L)
#define ISPU_DOUT_07_L       (ctrl_reg_base + 0x1E)
#define ISPU_DOUT_07_H       (ctrl_reg_base + 0x1F)
#define ISPU_DOUT_07         (ISPU_DOUT_07_L)
#define ISPU_DOUT_08_L       (ctrl_reg_base + 0x20)
#define ISPU_DOUT_08_H       (ctrl_reg_base + 0x21)
#define ISPU_DOUT_08         (ISPU_DOUT_08_L)
#define ISPU_DOUT_09_L       (ctrl_reg_base + 0x22)
#define ISPU_DOUT_09_H       (ctrl_reg_base + 0x23)
#define ISPU_DOUT_09         (ISPU_DOUT_09_L)
#define ISPU_DOUT_10_L       (ctrl_reg_base + 0x24)
#define ISPU_DOUT_10_H       (ctrl_reg_base + 0x25)
#define ISPU_DOUT_10         (ISPU_DOUT_10_L)
#define ISPU_DOUT_11_L       (ctrl_reg_base + 0x26)
#define ISPU_DOUT_11_H       (ctrl_reg_base + 0x27)
#define ISPU_DOUT_11         (ISPU_DOUT_11_L)
#define ISPU_DOUT_12_L       (ctrl_reg_base + 0x28)
#define ISPU_DOUT_12_H       (ctrl_reg_base + 0x29)
#define ISPU_DOUT_12         (ISPU_DOUT_12_L)
#define ISPU_DOUT_13_L       (ctrl_reg_base + 0x2A)
#define ISPU_DOUT_13_H       (ctrl_reg_base + 0x2B)
#define ISPU_DOUT_13         (ISPU_DOUT_13_L)
#define ISPU_DOUT_14_L       (ctrl_reg_base + 0x2C)
#define ISPU_DOUT_14_H       (ctrl_reg_base + 0x2D)
#define ISPU_DOUT_14         (ISPU_DOUT_14_L)
#define ISPU_DOUT_15_L       (ctrl_reg_base + 0x2E)
#define ISPU_DOUT_15_H       (ctrl_reg_base + 0x2F)
#define ISPU_DOUT_15         (ISPU_DOUT_15_L)

#define ISPU_INT1_CTRL1      (ctrl_reg_base + 0x50)
#define ISPU_INT1_CTRL2      (ctrl_reg_base + 0x51)
#define ISPU_INT1_CTRL       (ISPU_INT1_CTRL1)
#define ISPU_INT2_CTRL1      (ctrl_reg_base + 0x54)
#define ISPU_INT2_CTRL2      (ctrl_reg_base + 0x55)
#define ISPU_INT2_CTRL       (ISPU_INT2_CTRL1)
#define ISPU_INT_STATUS1     (ctrl_reg_base + 0x58)
#define ISPU_INT_STATUS2     (ctrl_reg_base + 0x59)
#define ISPU_INT_STATUS      (ISPU_INT_STATUS1)
#define ISPU_INT_PIN         (ctrl_reg_base + 0x5C)

#define ISPU_ALGO1           (ctrl_reg_base + 0x70)
#define ISPU_ALGO2           (ctrl_reg_base + 0x71)
#define ISPU_ALGO            (ISPU_ALGO1)

#define ISPU_ARAW_X_L        (ctrl_reg_base + 0x80)
#define ISPU_ARAW_X_M        (ctrl_reg_base + 0x81)
#define ISPU_ARAW_X_H        (ctrl_reg_base + 0x82)
#define ISPU_ARAW_X_HH       (ctrl_reg_base + 0x83)
#define ISPU_ARAW_X          (ISPU_ARAW_X_L)
#define ISPU_ARAW_Y_L        (ctrl_reg_base + 0x84)
#define ISPU_ARAW_Y_M        (ctrl_reg_base + 0x85)
#define ISPU_ARAW_Y_H        (ctrl_reg_base + 0x86)
#define ISPU_ARAW_Y_HH       (ctrl_reg_base + 0x87)
#define ISPU_ARAW_Y          (ISPU_ARAW_Y_L)
#define ISPU_ARAW_Z_L        (ctrl_reg_base + 0x88)
#define ISPU_ARAW_Z_M        (ctrl_reg_base + 0x89)
#define ISPU_ARAW_Z_H        (ctrl_reg_base + 0x8A)
#define ISPU_ARAW_Z_HH       (ctrl_reg_base + 0x8B)
#define ISPU_ARAW_Z          (ISPU_ARAW_Z_L)

#define ISPU_TEMP_L          (ctrl_reg_base + 0xA4)
#define ISPU_TEMP_H          (ctrl_reg_base + 0xA5)
#define ISPU_TEMP            (ISPU_TEMP_L)

#define ISPU_CALL_EN1        (ctrl_reg_base + 0xB8)
#define ISPU_CALL_EN2        (ctrl_reg_base + 0xB9)
#define ISPU_CALL_EN3        (ctrl_reg_base + 0xBA)
#define ISPU_CALL_EN4        (ctrl_reg_base + 0xBB)
#define ISPU_CALL_EN         (ISPU_CALL_EN1)

#define ctrl_reg_ext 0x80900

// EXT_CTRL[0] = 1 to enable write access for the following registers (where applicable)
// Note: write access is mutually exclusive with the device interface
#define EXT_CTRL             (ctrl_reg_ext + 0x00)

#define CTRL1                (ctrl_reg_ext + 0x04) // not writable
#define CTRL2                (ctrl_reg_ext + 0x05)
#define CTRL3                (ctrl_reg_ext + 0x06) // not writable
#define CTRL4                (ctrl_reg_ext + 0x07) // only bit 0 is writable

#define FIFO_CTRL1           (ctrl_reg_ext + 0x08) // not writable
#define FIFO_CTRL2           (ctrl_reg_ext + 0x09) // not writable
#define FIFO_CTRL3           (ctrl_reg_ext + 0x0A) // not writable

#define IF_CFG               (ctrl_reg_ext + 0x0F) // only bit 2 is writable

// to configure FIFO when FIFO_READ_FROM_ISPU = 1
#define FIFO_CTRL1_ISPU      (ctrl_reg_ext + 0x10)
#define FIFO_CTRL2_ISPU      (ctrl_reg_ext + 0x11)
#define FIFO_CTRL3_ISPU      (ctrl_reg_ext + 0x12)

#define SLEEPCNT_CFG         (ctrl_reg_ext + 0x17)

#define ISPU_CTRL1           (ctrl_reg_ext + 0x18) // only ISPU_RATE is writable
#define ISPU_CTRL2           (ctrl_reg_ext + 0x19) // not writable

#define TPF                  (ctrl_reg_ext + 0x20)

#define FIFO_STATUS1         (ctrl_reg_ext + 0x1C)
#define FIFO_STATUS2         (ctrl_reg_ext + 0x1D)

#define WHO_AM_I             (ctrl_reg_ext + 0x21)

#define TIMESTAMP0           (ctrl_reg_ext + 0x30)
#define TIMESTAMP1           (ctrl_reg_ext + 0x31)
#define TIMESTAMP2           (ctrl_reg_ext + 0x32)
#define TIMESTAMP3           (ctrl_reg_ext + 0x33)
#define TIMESTAMP4           (ctrl_reg_ext + 0x34)
#define TIMESTAMP            (TIMESTAMP0)

#define SLEEPCNT_TH_L        (ctrl_reg_ext + 0x3A)
#define SLEEPCNT_TH_H        (ctrl_reg_ext + 0x3B)
#define SLEEPCNT_TH          (SLEEPCNT_TH_L)

#define INTERNAL_FREQ_FINE   (ctrl_reg_ext + 0x47)

#define SLEEPCNT_TIME_L      (ctrl_reg_ext + 0x48)
#define SLEEPCNT_TIME_H      (ctrl_reg_ext + 0x49)
#define SLEEPCNT_TIME        (SLEEPCNT_TIME_L)

#define ISPU_DUMMY_CFG_1     (ctrl_reg_ext + 0x54)
#define ISPU_DUMMY_CFG_2     (ctrl_reg_ext + 0x55)
#define ISPU_DUMMY_CFG_3     (ctrl_reg_ext + 0x56)
#define ISPU_DUMMY_CFG_4     (ctrl_reg_ext + 0x57)
#define ISPU_DUMMY_CFG_5     (ctrl_reg_ext + 0x58)
#define ISPU_DUMMY_CFG_6     (ctrl_reg_ext + 0x59)
#define ISPU_DUMMY_CFG_7     (ctrl_reg_ext + 0x5A)
#define ISPU_DUMMY_CFG_8     (ctrl_reg_ext + 0x5B)
#define ISPU_DUMMY_CFG_9     (ctrl_reg_ext + 0x5C)
#define ISPU_DUMMY_CFG_10    (ctrl_reg_ext + 0x5D)
#define ISPU_DUMMY_CFG_11    (ctrl_reg_ext + 0x5E)
#define ISPU_DUMMY_CFG_12    (ctrl_reg_ext + 0x5F)
#define ISPU_DUMMY_CFG_13    (ctrl_reg_ext + 0x60)
#define ISPU_DUMMY_CFG_14    (ctrl_reg_ext + 0x61)
#define ISPU_DUMMY_CFG_15    (ctrl_reg_ext + 0x62)
#define ISPU_DUMMY_CFG_16    (ctrl_reg_ext + 0x63)

#define ISPU_LOPRIO_INT      (ctrl_reg_ext + 0x78) // clear only

#define ISPU2FIFO_FLAG       (ctrl_reg_ext + 0x7C)
#define FIFO2ISPU_TAG        (ctrl_reg_ext + 0x7D)
#define FIFO2ISPU_CTRL       (ctrl_reg_ext + 0x7E)
#define ISPU2FIFO_INT        (ctrl_reg_ext + 0x7F)

#endif

