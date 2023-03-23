## 1 - Introduction

This ISPU example implements the sensor fusion of the accelerometer, gyroscope and magnetometer, which is runtime calibrated.

The accelerometer and gyroscope are configured in high-performance mode at 104 Hz.

The magnetometer (LIS2MDL) is connected through the sensor-hub interface and it is configured in high-performance mode at 100 Hz. The sensor-hub configuration to apply is provided in the *output* folder as a separate file and must be loaded before the example main configuration.

The MotionFX and MotionMC libraries are provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Quaternion x-axis as float mapped starting from ISPU_DOUT_00_L (10h)
* Quaternion y-axis as float mapped starting from ISPU_DOUT_02_L (14h)
* Quaternion z-axis as float mapped starting from ISPU_DOUT_04_L (18h)
* Quaternion scalar component as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Magnetometer calibration level as uint8_t mapped starting from ISPU_DOUT_08_L (20h)
  * 0 is *unknown*
  * 1 is *poor*
  * 2 is *good*
  * 3 is *best*



## 2 - Device orientation

The accelerometer and gyroscope are mounted in ENU orientation.

The magnetometer is mounted in ESU orientation.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the quaternion for the new sample is computed and available in output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**