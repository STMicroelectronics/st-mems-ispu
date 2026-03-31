## 1 - Introduction

This ISPU example shows how to use the circular-buffer hardware accelerator by implementing a delay line for the accelerometer, configured in continuous mode at 2.5 kHz.

The outputs are as follows:

* Current accelerometer x-axis [LSB] as int32_t mapped starting from ISPU_DOUT_00_L (10h)
* Current accelerometer y-axis [LSB] as int32_t mapped starting from ISPU_DOUT_02_L (14h)
* Current accelerometer z-axis [LSB] as int32_t mapped starting from ISPU_DOUT_04_L (18h)
* Delayed accelerometer x-axis [LSB] as int32_t mapped starting from ISPU_DOUT_06_L (1Ch)
* Delayed accelerometer y-axis [LSB] as int32_t mapped starting from ISPU_DOUT_08_L (20h)
* Delayed accelerometer z-axis [LSB] as int32_t mapped starting from ISPU_DOUT_10_L (24h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the new delayed sample is available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
