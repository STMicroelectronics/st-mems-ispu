## 1 - Introduction

This ISPU example implements the wrist tilt solution based on the accelerometer data.

The accelerometer is configured in low-power mode at 26 Hz.

The MotionWT library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Wrist up event (1 if the user has tilted the wrist up) as uint8_t mapped starting from ISPU_DOUT_06_L (1Ch)
* Wrist down event (1 if the user has tilted the wrist down) as uint8_t mapped starting from ISPU_DOUT_06_H (1Dh)


## 2 - Device orientation

The accelerometer is mounted in ENU orientation.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the wrist tilt events for the new sample are computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
