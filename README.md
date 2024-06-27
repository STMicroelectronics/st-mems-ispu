# ISPU - intelligent sensor processing unit

This repository provides examples, tutorials, and other development resources for the **ISPU**, a dedicated ultralow-power, high-performance, programmable core, able to execute real-time processing directly inside the sensor.

The ISPU allows implementing algorithms written in C code running directly inside the sensor instead of on an application processor, enabling consistent reduction in power consumption, latency, and cost. Any type of algorithm can be implemented, from artificial intelligence to signal processing.

Thanks to the sensor hub functionality, which allows connecting up to four external sensors, the ISPU can process their data as well, thus enhancing the intelligence of sensors that do not have any onboard processing.

The output of the algorithms can be read from the application processor at any time. Furthermore, there is the possibility to generate an interrupt when there is new relevant information in the output, so that the application processor can otherwise sleep and save power.

The sensors embedding the ISPU are supported by the [ISPU-Toolchain](https://www.st.com/en/development-tools/ispu-toolchain.html), the [X-CUBE-ISPU](https://www.st.com/en/embedded-software/x-cube-ispu.html) software package, [MEMS-Studio](https://www.st.com/en/development-tools/mems-studio.html), a graphical application to develop and test solutions, and the software included in this repository. Sensors with the part number ending in "IS**N**" are also supported by [NanoEdge AI Studio](https://www.st.com/en/development-tools/nanoedgeaistudio.html), a tool allowing anyone to easily create machine learning solutions.

Pretrained artificial intelligence models can be easily converted to code optimized for the ISPU using [ST Edge AI Core](https://www.st.com/en/development-tools/stedgeai-core.html). Other tools from the [ST Edge AI Suite](https://www.st.com/content/st_com/en/st-edge-ai-suite.html) that are based on ST Edge AI Core can also be used: [MEMS-Studio](https://www.st.com/en/development-tools/mems-studio.html) enables a graphical approach with its ISPU Model Converter feature, while the [ST Edge AI Developer Cloud](https://www.st.com/en/development-tools/stedgeai-dc.html) allows converting and benchmarking models with only a web browser.

For more information, please explore the page on ST's website dedicated to the [MEMS Sensors Ecosystem for Machine Learning](https://www.st.com/content/st_com/en/ecosystems/MEMS-Sensors-Ecosystem-for-Machine-Learning.html).

[![ISPU introduction video](_media/ispu_intro.gif)](https://youtu.be/6WDKwd7kke0)

## Repository overview

This repository is structured as follows:

- An [examples](https://github.com/STMicroelectronics/ispu-examples) folder, containing templates and example projects as well as libraries to start programming with the ISPU, together with prebuilt files ready to be used with the sensors. Additionally, it contains instructions on how to set up the development environment.
- A [tutorials](./tutorials/) folder, containing tutorials describing how to create example solutions using different ST hardware kits and software tools.

Here is where to find the resources helpful when using [ST Edge AI Core](https://www.st.com/en/development-tools/stedgeai-core.html), [MEMS-Studio](https://www.st.com/en/development-tools/mems-studio.html)'s ISPU Model Converter, and the [ST Edge AI Developer Cloud](https://www.st.com/en/development-tools/stedgeai-dc.html):

 - Templates for integration into the final application, available under the [examples](https://github.com/STMicroelectronics/ispu-examples) folder.
 - Templates for validation on target, available under the [examples](https://github.com/STMicroelectronics/ispu-examples) folder.
 - Host board firmware for validation on target, available under the [host_firmware](./host_firmware/) folder.

------

**More information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2023 STMicroelectronics**
