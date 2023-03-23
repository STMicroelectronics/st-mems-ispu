## 1 - Introduction

This ISPU example implements the pedometer solution based on the accelerometer data.

The accelerometer is configured in low-power mode at 26 Hz.

The MotionPM library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Number of steps as uint16_t mapped starting from ISPU_DOUT_06_L (1Ch)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the number of steps for the new sample are computed and available in output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
