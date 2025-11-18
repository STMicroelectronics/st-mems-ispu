## 1 - Introduction

This ISPU example implements the free-fall detection algorithm.

The accelerometer is configured in high-performance mode at 104 Hz.

The code which implements the free-fall detection algorithm is provided as source in the *ispu/src/free_fall.c* file.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Free-fall status as uint8_t mapped starting from ISPU_DOUT_06_L (1Ch)
  * 0 is *free-fall not detected*
  * 1 is *free-fall detected*


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the free-fall status for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2024 STMicroelectronics**
