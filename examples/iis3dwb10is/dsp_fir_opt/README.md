## 1 - Introduction

This ISPU example shows how to use the *ispu_fir_f32_opt()* function of the ISPU DSP package.
The accelerometer is configured in continuous mode at 2.5 kHz.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Filtered accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Filtered accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_08_L (20h)
* Filtered accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_10_L (24h)

This example streams the output data using the ISPU output registers for demonstration purposes.
Writing output data to FIFO is likely necessary in real applications to support high data rates.


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when a new sample is available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
