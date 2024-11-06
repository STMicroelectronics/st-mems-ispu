# ST Edge AI validation on target firmware

This firmware allows performing the validation on target (that is, on the ISPU) of neural network models converted with [ST Edge AI Core](https://www.st.com/en/development-tools/stedgeai-core.html).

## Supported hardware and how to configure it

The firmware supports the combination of the following hardware:
 - Nucleo board: [NUCLEO-F401RE](https://www.st.com/en/evaluation-tools/nucleo-f401re.html), [NUCLEO-U575ZI-Q](https://www.st.com/en/evaluation-tools/nucleo-u575zi-q.html)
 - Expansion board: [X-NUCLEO-IKS01A3](https://www.st.com/en/ecosystems/x-nucleo-iks01a3.html), [X-NUCLEO-IKS02A1](https://www.st.com/en/ecosystems/x-nucleo-iks02a1.html), [X-NUCLEO-IKS4A1](https://www.st.com/en/ecosystems/x-nucleo-iks4a1.html)
 - Sensor adapter board (only for X-NUCLEO-IKS01A3 and X-NUCLEO-IKS02A1): [STEVAL-MKI229A](https://www.st.com/en/evaluation-tools/steval-mki229a.html), [STEVAL-MKI230KA](https://www.st.com/en/evaluation-tools/steval-mki230ka.html), [STEVAL-MKI233KA](https://www.st.com/en/evaluation-tools/steval-mki233ka.html)

Regarding the expansion board, make sure that the INT2 pin of the sensor is correctly routed, otherwise the measurement of the inference time of the model will not work:
 - On the X-NUCLEO-IKS01A3, JP6 (*USER_INT* routing selector) must be positioned to connect pins 13 and 14 to route *M_INT2_O*, the INT2 pin of the DIL24 adapter socket
 - On the X-NUCLEO-IKS02A1, JP6 (*USER_INT* routing selector) must be positioned to connect pins 5 and 6  to route *M_INT2_O*, the INT2 pin of the DIL24 adapter socket
 - On the X-NUCLEO-IKS4A1, the on-board LSM6DSO16IS sensor is used and routing the INT2 pin is not dependent on any jumper configuration

If using the X-NUCLEO-IKS4A1, do not plug any sensor adapter board in the DIL24 socket if it hosts a sensor with the same I2C address as the LSM6DSO16IS sensor already available on the expansion board, as a clash of addresses does not allow the firmware to work correctly. If a sensor with the same I2C address must necessarily be plugged in the DIL24 socket (not needed for the purpose of this firmware), then SB34, SB35, SB43 and SB44 soldering bridges must be modified to connect the SA0 pin of the DIL24 socket to VDD and the SA0 pin of the LSM6DSO16IS sensor of the expansion board to ground, or viceversa, in order to differentiate the I2C addresses.

 Note: make sure that the Nucleo board ST-LINK firmware is updated to the latest version, as it may affect the correct functioning of the communication between the board and the PC. To check the ST-LINK firmware version and upgrade it, you may use the [dedicated upgrade utility](https://www.st.com/en/development-tools/stsw-link007.html) or other ST tools that embed it (for example, [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) or [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)).

## Prebuilt binaries, build and porting

The *binary* folder contains the prebuilt firmware for the supported Nucleo boards. The board can be flashed by simply copying the binary file to the Nucleo board mass storage or by using [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html). Note that the firmware may be flashed only once (instead of every time a validation on target needs to be performed), as it automatically adapts to the model under validation.

The source project for each supported Nucleo board is also available in the dedicated folders. The projects can be built using either the Makefile (with the GNU Embedded Toolchain for ARM installed) or the [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) project. The projects can be generated for other development enviroments with [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) using the provided *.ioc* files.

The source code may also be used to port the firmware to a non-supported hardware setup. The firmware was generated using [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html). All custom code is contained in *Core/Inc/applontication.h* and *Core/Src/application.c* (the *application* function is called from *Core/Src/main.c* in the main loop), thus allowing for easy porting, without modifying much of *Core/Src/application.c*. Modifications might be necessary for code related to hardware-dependent resources, such as timers, interrupt pins, and communication interfaces.

## Validation template and validation flow

In order to perform the validation on target, a template to integrate the converted model and communicate with the Nucleo validation firmware is needed. The template for the specific sensors can be found in the [examples](https://github.com/STMicroelectronics/ispu-examples) folder of this repository.

For more information on all the steps required to perform the validation on target, refer to the readme file of the validation template.

------

**More information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2024 STMicroelectronics**
