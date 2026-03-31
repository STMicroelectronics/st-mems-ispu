## 1 - Introduction

This ISPU example shows how to use the *ispu_absmax_no_idx_f32()* function of the ISPU DSP package.
The accelerometer is configured in continuous mode at 2.5 kHz.

The outputs are as follows:

* Accelerometer x-axis maximum of absolute values [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis maximum of absolute values [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis maximum of absolute values [g] as float mapped starting from ISPU_DOUT_04_L (18h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the features of the new window are computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
