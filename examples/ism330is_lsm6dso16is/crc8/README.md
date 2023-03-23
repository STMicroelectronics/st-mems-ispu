## 1 - Introduction

This ISPU example implements the accelerometer and gyroscope data cyclic redundancy check (CRC) computation.

The accelerometer and gyroscope are configured in high-performance mode at 26 Hz.

The 8-bit CRC (CRC8) of the sensor data (12 bytes) is computed.

The code which implements the CRC computation is provided as source in the *ispu/src/crc8.c* file.

The outputs are as follows:

* Accelerometer x-axis [LSB] as int16_t mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [LSB] as int16_t mapped starting from ISPU_DOUT_01_L (12h)
* Accelerometer z-axis [LSB] as int16_t mapped starting from ISPU_DOUT_02_L (14h)
* Gyroscope x-axis [LSB] as int16_t mapped starting from ISPU_DOUT_03_L (16h)
* Gyroscope y-axis [LSB] as int16_t mapped starting from ISPU_DOUT_04_L (18h)
* Gyroscope z-axis [LSB] as int16_t mapped starting from ISPU_DOUT_05_L (1Ah)
* CRC8 as uint8_t mapped starting from ISPU_DOUT_06_L (1Ch)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the CRC for the new sample is computed and available in output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**