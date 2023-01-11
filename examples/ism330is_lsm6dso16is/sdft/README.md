## 1 - Introduction

This ISPU example implements the sliding discrete Fourier transform (SDFT) algorithm for computing the magnitude of the DFT components of the accelerometer z-axis over a sliding window of 26 samples.

The accelerometer is configured in high-performance mode at 26 Hz.

The sdft library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer z-axis sample as float mapped starting from ISPU_DOUT_00_L (10h)
* DFT magnitudes as an array of 14 float values mapped starting from ISPU_DOUT_02_L (14h)

## 2 - Device orientation

None.

## 3 - Interrupts

The configuration generates an interrupt on INT1 when the DFT magnitudes are computed for the new sample and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2023 STMicroelectronics**