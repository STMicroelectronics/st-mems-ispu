## 1 - Introduction

This ISPU example implements the fitness activity solution based on the fusion of the accelerometer, gyroscope and pressure sensor data, which counts the repetitions of every exercize.

The accelerometer and gyroscope are configured in low-power mode at 26 Hz.

The pressure sensor (LPS22HH) is connected through the sensor-hub interface and it is configured in low-noise mode at 25 Hz. The sensor-hub configuration to apply is provided in the *output* folder as a separate file and must be loaded before the example main configuration.

The MotionFA library is provided as binary in the *ispu/lib* folder.

The outputs are as follows:

* Accelerometer x-axis [g] as float mapped starting from ISPU_DOUT_00_L (10h)
* Accelerometer y-axis [g] as float mapped starting from ISPU_DOUT_02_L (14h)
* Accelerometer z-axis [g] as float mapped starting from ISPU_DOUT_04_L (18h)
* Gyroscope x-axis [g] as float mapped starting from ISPU_DOUT_06_L (1Ch)
* Gyroscope y-axis [g] as float mapped starting from ISPU_DOUT_08_L (20h)
* Gyroscope z-axis [g] as float mapped starting from ISPU_DOUT_10_L (24h)
* Pressure [hPa] as float mapped starting from ISPU_DOUT_12_L (28h)
* Biceps curls counter [#] as uint16_t mapped starting from ISPU_DOUT_14_L (2Ch)
* Squats counter [#] as uint16_t mapped starting from ISPU_DOUT_15_L (2Eh)
* Push-ups counter [#] as uint16_t mapped starting from ISPU_DOUT_16_L (30h)
* Lateral raises counter [#] as uint16_t mapped starting from ISPU_DOUT_17_L (32h)
* Overhead presses counter [#] as uint16_t mapped starting from ISPU_DOUT_18_L (34h)
* Jumping jacks counter [#] as uint16_t mapped starting from ISPU_DOUT_19_L (36h)


## 2 - Device orientation

The accelerometer and gyroscope are mounted in ENU orientation on the left wrist.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the fitness activity counters for the new sample are computed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**