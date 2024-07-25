## 1 - Introduction

This ISPU example implements the standing vs sitting solution based on the fusion of the accelerometer and pressure sensor data.

The accelerometer is configured in low-power mode at 26 Hz.

The pressure sensor (LPS22HH) is connected through the sensor-hub interface and it is configured in low-noise mode at 25 Hz. The sensor-hub configuration to apply is provided in the *output* folder as a separate file and must be loaded before the example main configuration.

The MotionSD library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Pressure [hPa] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Standing vs sitting identifier as uint8_t mapped starting from ISPU_DOUT_08_L (20h)
  * 0 is *unknown*
  * 1 is *sitting*
  * 2 is *standing*


## 2 - Device orientation

The accelerometer is mounted in ENU orientation on the wrist.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the standing vs sitting posture for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
