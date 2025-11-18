## 1 - Introduction

This ISPU example implements the inclinometer solution based on the accelerometer calibration and the accelerometer and gyroscope sensor fusion.

The accelerometer and gyroscope are configured in high-performance mode at 52 Hz.

The MotionAC and MotionFX libraries are provided as binaries in the *ispu/lib* folder.

The outputs are as follows:

* Pitch angle [deg] as float mapped starting from ISPU_DOUT_00_L (10h)
* Roll angle [deg] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer calibration level as uint8 mapped starting from ISPU_DOUT_04_L (18h)
  * 0 is *unknown*
  * 1 is *poor*
  * 2 is *ok*
  * 3 is *good*

The accelerometer calibration procedure requires acquiring the accelerometer samples at six different orientations with the device steady for a couple of seconds. The six positions can be with every axis towards the ceiling and the ground such that each axis senses about +1 g or -1 g.

## 2 - Device orientation

The accelerometer and gyroscope are mounted in ENU orientation.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the pitch and roll angles for the new sample are computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2024 STMicroelectronics**
