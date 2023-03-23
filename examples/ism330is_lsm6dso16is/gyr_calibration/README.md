## 1 - Introduction

This ISPU example implements the gyroscope calibration.

The accelerometer and the gyroscope are configured in high-performance mode at 52 Hz.

The MotionGC library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Calibrated gyroscope x-axis offset [dps] as float mapped starting from ISPU_DOUT_00_L (10h)
* Calibrated gyroscope y-axis offset [dps] as float mapped starting from ISPU_DOUT_02_L (14h)
* Calibrated gyroscope z-axis offset [dps] as float mapped starting from ISPU_DOUT_04_L (18h)
* Gyroscope x-axis offset [dps] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Gyroscope y-axis offset [dps] as float mapped starting from ISPU_DOUT_08_L (20h)
* Gyroscope z-axis offset [dps] as float mapped starting from ISPU_DOUT_10_L (24h)

The calibration procedure requires keeping the device steady.


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the calibrated gyroscope for the new sample is computed and available in output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
