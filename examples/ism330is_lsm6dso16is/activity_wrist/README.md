## 1 - Introduction

This ISPU example implements the activity recognition for a wrist worn device.

The accelerometer is configured in high-performance mode at 26 Hz.

The MotionAW library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Activity recognition identifier as uint8_t mapped starting from ISPU_DOUT_06_L (1Ch)
  * 0 is *no activity*
  * 1 is *stationary*
  * 2 is *standing*
  * 3 is *sitting*
  * 4 is *lying*
  * 5 is *walking*
  * 6 is *fast walking*
  * 7 is *jogging*
  * 8 is *biking*
  * 9 is *others*

* Confidence level (in the range 0 to 5, from low to high) as uint8_t mapped starting from ISPU_DOUT_06_H (1Dh)


## 2 - Device orientation

The accelerometer is mounted in ENU orientation.

## 3 - Interrupts

The configuration generates an interrupt on INT1 when the activity for the new sample is computed and available in output registers.

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
