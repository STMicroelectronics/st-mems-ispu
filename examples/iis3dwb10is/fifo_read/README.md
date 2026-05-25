## 1 - Introduction

This ISPU example implements the reading of accelerometer data from the FIFO. For each accelerometer axis, the mean value is computed over the number of samples configured with the FIFO watermark threshold (or the entire FIFO if the FIFO watermark threshold is set to 0).

The outputs are as follows:

* Accelerometer x-axis mean [LSB] as int32_t mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis mean [LSB] as int32_t mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis mean [LSB] as int32_t mapped starting from ISPU_DOUT_04_L (18h)
* Number of samples [#] as uint16_t mapped starting from ISPU_DOUT_06_L (1Ch)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the mean for the new set of FIFO samples is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
