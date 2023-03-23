## 1 - Introduction

This is a template for creating a new project for ISPU.

The *ispu* folder may be copied by itself (no dependencies other than the CLI toolchain or IDE environment that must be installed) and used as a starting blank project to implement any solution running on ISPU.


## 2 - Structure and how to use

The *ispu* folder contains:

* ***src***:
  * *main.c*: main source file template. This is where the user code must be implemented:
    * The *main* function should not be modified.
    * The user should fill in *algo_00_init* and *algo_00* respectively with the initialization and running code of the algorithm.
    * Please note that analogous functions (e.g., *algo_01_init* and *algo_01*) can be added in order to have additional algorithms (up to 30).
* ***ispu_utils***: folder containing several utilities. Usually, there is no need to modify these files.
  * *boot.ld*: linker script.
  * *crt0.S*: startup file.
  * *global.S*: additional assembly file.
  * *reg_map.h*: register map defines.
  * *peripherals.h*: cast defines and others.
* ***conf.txt***: file containing sensor configuration. Please refer to the help of the *ispu_gen* command line tool (included in the toolchain or Eclipse IDE) for how to modify the configuration.
* ***make***: folder containing the makefile for building using the command line.
* ***eclipse***: folder containing the Eclipse project.

## 3 - Output data format definition

Since the format of the output data written to the ISPU_DOUT registers depends on the specific user code, STMicroelectronics tools (e.g., Unicleo-GUI) utilize JSON files to define the output data format with a specific syntax.

An "output" key must be defined at the highest level of the JSON structure. The "output" key must contain an array. Each element of the array must describe one output. Each element must have a "name" key, a "type" key and, optionally, a "size" key:

- The value of the "name" key can be any string defining a label for the element.

- The value of the "type" key must be a string representing one of the following data types:
  - uint8_t
  - int8_t
  - uint16_t
  - int16_t
  - uint32_t
  - int32_t
  - float
  - char
- If the output is an array, the value of the "size" key must be integer representing the array size.

For example, supposing that the user code copies the accelerometer raw values to ISPU_DOUT_00, ISPU_DOUT_01 and ISPU_DOUT_02 registers, the JSON file below defines three elements containing the three axes of the accelerometer in LSB, written to the output registers as 16-bit words in two's complement (int16_t data type).

```json
{
  "output":
  [
    {
      "name": "Acc x [LSB]",
      "type": "int16_t"
    },
    {
      "name": "Acc y [LSB]",
      "type": "int16_t"
    },
    {
      "name": "Acc z [LSB]",
      "type": "int16_t"
    }
  ]
}
```

An equivalent definition can be achieved by defining one single element as an array of size three.

```json
{
  "output":
  [
    {
      "name": "Acc [LSB]",
      "type": "int16_t",
      "size": 3
    }
  ]
}
```

------

**More Information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2022 STMicroelectronics**