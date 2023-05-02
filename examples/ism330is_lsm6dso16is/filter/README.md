## 1 - Introduction

This ISPU example implements the accelerometer data filtering.

The accelerometer is configured in high-performance mode at 26 Hz.

The accelerometer axes are filtered using a second order IIR low-pass filter with 1 Hz cutoff frequency.

The code which implements the filter (second order IIR) is provided as source in the *ispu/src/iir2.c* file.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Accelerometer filtered x-axis [g] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Accelerometer filtered y-axis [g] as float mapped starting from ISPU_DOUT_08_L (20h)
* Accelerometer filtered z-axis [g] as float mapped starting from ISPU_DOUT_10_L (24h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the filtered accelerometer data for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**