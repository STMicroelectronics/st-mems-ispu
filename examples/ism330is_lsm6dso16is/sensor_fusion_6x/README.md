## 1 - Introduction

This ISPU example implements the sensor fusion of the accelerometer and gyroscope, configured in high-performance mode at 104 Hz.

The MotionFX library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Quaternion x-axis as float mapped starting from ISPU_DOUT_00_L (10h)
* Quaternion y-axis as float mapped starting from ISPU_DOUT_02_L (14h)
* Quaternion z-axis as float mapped starting from ISPU_DOUT_04_L (18h)
* Quaternion scalar component as float mapped starting from ISPU_DOUT_06_L (1Ch)


## 2 - Device orientation

The accelerometer and gyroscope are mounted in ENU orientation.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the quaternion for the new sample is computed and available in output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**