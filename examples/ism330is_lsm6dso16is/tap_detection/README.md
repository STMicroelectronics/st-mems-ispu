## 1 - Introduction

This ISPU example implements the tap detection solution based on the accelerometer data.

The accelerometer is configured in high-performance mode at 416 Hz.

The MotionTD library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Tap event as uint8_t mapped starting from ISPU_DOUT_06_L (1Ch)
  * 0 is *none* (no tap is detected)
  * 1 is *single* (single tap is detected)
  * 2 is *double* (double tap is detected)
  * 3 is *triple* (triple tap is detected)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the tap event for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
