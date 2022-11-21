## 1 - Introduction

This ISPU example implements the vibration severity solution, where the velocity RMS is computed on one selected accelerometer axis filtered according to ISO10816.

The accelerometer is configured in high-performance mode at 6667 Hz.

The MotionVS library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Acceleration [mm / s^2] as float mapped starting from ISPU_DOUT_00_L (10h)
* Bandpass filtered acceleration [mm / s^2] as float mapped starting from ISPU_DOUT_02_L (14h)
* Velocity [mm / s] as float mapped starting from ISPU_DOUT_04_L (18h)
* Velocity RMS [mm / s] as float mapped starting from ISPU_DOUT_06_L (1Ch)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the filtered acceleration, velocity, and velocity RMS for the new sample are computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**