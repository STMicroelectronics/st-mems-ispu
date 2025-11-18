## 1 - Introduction

This ISPU example implements the 6D detection algorithm.

The accelerometer is configured in high-performance mode at 26 Hz.

The code which implements the 6D detection algorithm is provided as source in the *ispu/src/detect_6d.c* file.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* 6D orientation status as uint8_t mapped starting from ISPU_DOUT_06_L (1Ch)
  * 0 is *unknown orientation*
  * 1 is *x-axis mostly pointing down*
  * 2 is *x-axis mostly pointing up*
  * 3 is *y-axis mostly pointing down*
  * 4 is *y-axis mostly pointing up*
  * 5 is *z-axis mostly pointing down*
  * 6 is *z-axis mostly pointing up*


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the 6D orientation status for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2024 STMicroelectronics**
