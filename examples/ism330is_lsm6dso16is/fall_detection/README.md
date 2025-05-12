## 1 - Introduction

This ISPU example implements the fall detection solution based on the fusion of the accelerometer and pressure sensor data.

The accelerometer is configured in low-power mode at 104 Hz.

The pressure sensor (LPS22DF) is connected through the sensor-hub interface and it is configured at 10 Hz with an averaging setting of 8 and a low-pass filter of ODR/9 bandwidth. The sensor-hub configuration for this sensor is defined in the *ispu/shub.txt* file.

The MotionFD library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Pressure [hPa] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Fall event (1 if the user has fallen) as uint8_t mapped starting from ISPU_DOUT_08_L (20h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the fall event for the new sample is computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**