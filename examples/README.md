## 1 - Introduction

This software package provides template and example projects as well as libraries for MEMS devices embedding the ISPU (intelligent sensor processing unit).

Each project can be built using the command line (make) or any Eclipse-based IDE, including STM32CubeIDE, provided that the ISPU plugins are installed.

Prebuilt files containing the ready-to-use device configuration are available in the *output* folder of each example project. For the examples related to STMicroelectronics libraries, the library in binary format is also available to be integrated in other projects based on user needs.

## 2 - Command line

In order to build a project using the command line, the ISPU toolchain must be installed on the system. Besides the toolchain, the *make* build tool must be installed.

The toolchain can be downloaded for Windows, Linux and macOS at the following link: [https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/sensor-software-development-tools/ispu-toolchain.html](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/sensor-software-development-tools/ispu-toolchain.html).

The toolchain must be extracted from the downloaded archive to any folder on the system. The *bin* folder inside the installed toolchain must be added to the PATH environment variable as follows:

* On Windows:

  * Open the Start search, type "env" and choose "Edit environment variables for your account".
  * Under the "User variables" section, find the row with "Path" in the "Variable" column, select it and click "Edit...".
  * Click on the "New" button to add a new entry.
  * Click on the "Browse..." button, browse to the folder where the toolchain is installed, select the *bin* folder and click "OK".
  * Close all remaining windows by clicking "OK".
  * Open a new terminal. The toolchain is now available to be called from the command line (for example, the *reisc-gcc* command should be found if the user tries to run it).

* On Linux and macOS:

  * Add to your ".bashrc" or equivalent shell startup file the following line:

    ```shell
    export PATH=$PATH":/path/to/toolchain/bin"
    ```

  * Open a new terminal. The toolchain is now available to be called from the command line (for example, the *reisc-gcc* command should be found if the user tries to run it).

Once the toolchain is installed, in order to build a project, the user must enter the *ispu/make* directory inside the project and run *make*. Enjoy.

## 3 - Eclipse

In order to build a project using an Eclipse-based IDE, the ISPU plugins must be installed in the IDE that the user desires to use.

If an Eclipse-based IDE is not already available, please download and install the "Eclipse IDE for Embedded C/C++ Developers" from [https://www.eclipse.org](https://www.eclipse.org).

In order to install the plugins, from the main menu, go to "Help", click on "Install New Software...", click on "Add...", insert the URL corresponding to the operating system used in the "Location" field and click the "Add" button. Then, select all the items, click on the "Next" button, accept the license terms, and click on the "Finish" button. When prompted, restart the IDE to complete the installation.

The URL to use is:

* On Windows: [https://sw-center.st.com/mems/ispu/ispu_repository_win](https://sw-center.st.com/mems/ispu/ispu_repository_win)
* On Linux: [https://sw-center.st.com/mems/ispu/ispu_repository_linux](https://sw-center.st.com/mems/ispu/ispu_repository_linux)
* On macOS: [https://sw-center.st.com/mems/ispu/ispu_repository_mac](https://sw-center.st.com/mems/ispu/ispu_repository_mac)

Once the plugins are installed, in order to build a project, the user must go to "File", click on "Import...". Under "General", select "Existing Project into Workspace", click on the "Next" button, click on the "Browse" button, select the folder containing the project (*ispu/eclipse* directory), and click on the "Finish" button. The project is now imported and can be built. Enjoy.

------

**More information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**
