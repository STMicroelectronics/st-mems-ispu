## 1 - Introduction

This ISPU example shows how to use the accumulate-and-multiply hardware instruction by implementing the computation of the dot product between the current and previous measurements of the accelerometer, configured in continuous mode at 2.5 kHz.

The *dot* function is implemented in the *dot.S* file.

This example is intended as a minimal example to show the accumulate-and-multiply hardware instruction. The performance improvement achieved with the usage of this instruction depends on the length of the input vector.

The outputs are as follows:

* Current accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Current accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Current accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Previous accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Previous accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_08_L (20h)
* Previous accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_10_L (24h)
* Accelerometer dot product [g^2] as float mapped starting from ISPU_DOUT_12_L (28h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the dot product for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2026 STMicroelectronics**
