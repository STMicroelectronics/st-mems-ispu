## 1 - Introduction

This ISPU example implements a looping message written to the output registers using the "printf" function. The example shows how to overwrite the "\_write" function in order to make the printf write to the output registers.

The output is as follows:

* Message (max 64 characters including the null terminator) as char array mapped starting from ISPU_DOUT_00_L (10h)


## 2 - Device orientation

None.


## 3 - Interrupts

The configuration generates an interrupt on INT1 when the message for the new sample is printed and available in the output registers.

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**