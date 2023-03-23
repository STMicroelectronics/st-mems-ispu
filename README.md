# ISPU - intelligent sensor processing unit

This repository provides examples, tutorials, and other development resources for the **ISPU**, a dedicated ultralow-power, high-performance, programmable core, able to execute real-time processing directly inside the sensor.

The ISPU allows implementing algorithms written in C code running directly inside the sensor instead of on an application processor, enabling consistent reduction in power consumption, latency, and cost. Any type of algorithm can be implemented, from artificial intelligence to signal processing.

Thanks to the sensor hub functionality, which allows connecting up to four external sensors, the ISPU can process their data as well, thus enhancing the intelligence of sensors that do not have any onboard processing.

The output of the algorithms can be read from the application processor at any time. Furthermore, there is the possibility to generate an interrupt when there is new relevant information in the output, so that the application processor can otherwise sleep and save power.

The sensors embedding the ISPU are supported by the [ISPU-Toolchain](https://www.st.com/en/development-tools/ispu-toolchain.html), the [X-CUBE-ISPU](https://www.st.com/en/embedded-software/x-cube-ispu.html) software package, and the software included in this repository. Sensors with the part number ending in "IS**N**" are also supported by [NanoEdge AI Studio](https://www.st.com/en/development-tools/nanoedgeaistudio.html), a tool allowing anyone to easily create machine learning solutions.

For more information, please explore the dedicated page on ST's website: [https://www.st.com/ispu](https://www.st.com/ispu).

[![ISPU introduction video](_media/ispu_intro.gif)](https://youtu.be/6WDKwd7kke0)

## Repository overview

This repository is structured as follows:

- An [examples](https://github.com/STMicroelectronics/ispu-examples) folder, containing templates and example projects as well as libraries to start programming with the ISPU, together with prebuilt files ready to be used with the sensors. Additionally, it contains instructions on how to set up the development environment.
- A [tutorials](./tutorials/) folder, containing tutorials describing how to create example solutions using different ST hardware kits and software tools.

------

**More information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2023 STMicroelectronics**
