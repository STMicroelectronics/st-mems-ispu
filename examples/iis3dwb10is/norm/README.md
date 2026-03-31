## 1 - Introduction

This ISPU example implements the computation of the norm of the three axes of the accelerometer, configured in continuous mode at 2.5 kHz.

The outputs are as follows:

* Accelerometer x-axis [LSB] as int32_t mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [LSB] as int32_t mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [LSB] as int32_t mapped starting from ISPU_DOUT_04_L (18h)
* Accelerometer norm [LSB] as float mapped starting from ISPU_DOUT_06_L (1Ch)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the norm for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
