## 1 - Introduction

This ISPU example implements the accelerometer calibration.

The accelerometer is configured in high-performance mode at 52 Hz.

The MotionAC library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Calibrated accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Calibrated accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Calibrated accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Accelerometer x-axis offset [g] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Accelerometer y-axis offset [g] as float mapped starting from ISPU_DOUT_08_L (20h)
* Accelerometer z-axis offset [g] as float mapped starting from ISPU_DOUT_10_L (24h)
* Accelerometer x-axis scale factor as float mapped starting from ISPU_DOUT_12_L (28h)
* Accelerometer y-axis scale factor as float mapped starting from ISPU_DOUT_14_L (2Ch)
* Accelerometer z-axis scale factor as float mapped starting from ISPU_DOUT_16_L (30h)
* Accelerometer calibration level as uint8 mapped starting from ISPU_DOUT_18_L (34h)
  * 0 is *unknown*
  * 1 is *poor*
  * 2 is *ok*
  * 3 is *good*


The calibration procedure requires acquiring the accelerometer samples at six different orientations with the device steady for a couple of seconds. The six positions can be with every axis towards the ceiling and the ground such that each axis senses about +1 g or -1 g.


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the calibrated accelerometer for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**