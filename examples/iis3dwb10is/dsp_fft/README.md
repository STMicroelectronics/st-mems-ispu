## 1 - Introduction

This ISPU example shows how to use the *ispu_rfft_f32()* function of the ISPU DSP package.
The accelerometer is configured in continuous mode at 40 kHz.

The outputs are as follows:

* Magnitude of the maximum energy bin of the accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Magnitude of the maximum energy bin of the accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Magnitude of the maximum energy bin of the accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Frequency of the maximum energy bin of the accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Frequency of the maximum energy bin of the accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_08_L (20h)
* Frequency of the maximum energy bin of the accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_10_L (24h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the features of the new FFT are computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
