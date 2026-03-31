## 1 - Introduction

This ISPU example implements the writing of accelerometer data from the ISPU to the FIFO.

The outputs are as follows:

* FIFO tag with value 01h mapped at FIFO_DATA_OUT_TAG (40h)
* Accelerometer x-axis [LSB] as 24-bit signed integer mapped starting from FIFO_DATA_OUT_D0 (41h)
* Accelerometer y-axis [LSB] as 24-bit signed integer mapped starting from FIFO_DATA_OUT_D3 (44h)
* Accelerometer z-axis [LSB] as 24-bit signed integer mapped starting from FIFO_DATA_OUT_D6 (47h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the FIFO reaches the watermark value (set to 1).

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
