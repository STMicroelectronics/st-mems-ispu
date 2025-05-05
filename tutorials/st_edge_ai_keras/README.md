# ST Edge AI for ISPU tutorial using Keras

This tutorial demonstrates the complete flow for deploying a neural network trained with Keras inside a sensor embedding the ISPU using [ST Edge AI Core](https://www.st.com/en/development-tools/stedgeai-core.html).

The use case consists of a human activity recognition (HAR) using accelerometer data to train a small neural network (NN) to recognize four activities: stationary, walking, running, and cycling.

## Requirements

For reproducing all the steps in this tutorial project, the following hardware and software components are required.

### Hardware

- [NUCLEO-F401RE](https://www.st.com/en/evaluation-tools/nucleo-f401re.html): STM32 Nucleo board supported by ST tools and software packages.

    ![NUCLEO-F401RE](images/nucleo-f401re.png)

- [X-NUCLEO-IKS4A1](https://www.st.com/en/ecosystems/x-nucleo-iks4a1.html): motion MEMS and environmental sensors expansion board, which must be connected to the NUCLEO-F401RE through the Arduino UNO V3 connector.

    ![X-NUCLEO-IKS4A1](images/x-nucleo-iks4a1.png)

### Software

- [ISPU-Toolchain](https://www.st.com/en/development-tools/ispu-toolchain.html): toolchain to build applications for sensors embedding the ISPU, which must be added to the system PATH (for detailed steps refer to the dedicated [README](https://github.com/STMicroelectronics/ispu-examples/README.md)).
- [ST Edge AI Core](https://www.st.com/en/development-tools/stedgeai-core.html): tool to easily convert pretrained AI models for integration into ST products, which must be added to the system PATH (for detailed steps refer to the "Setting environment" article of the documentation available within the ST Edge AI Core installation folder).
- [GNU Make](https://www.gnu.org/software/make): command line build tool, which must be added to the system PATH.
- [MEMS Studio](https://www.st.com/en/development-tools/MEMS-Studio.html): desktop software solution to develop and test solutions for MEMS sensors.
- [X-CUBE-ISPU](https://www.st.com/en/embedded-software/x-cube-ispu.html): expansion package for ISPU development containing documentation, examples, and STM32 firmware for enabling communication with MEMS Studio.
- [Python](https://www.python.org): Python programming language interpreter, necessary to create and train the neural network model (required Python packages are listed in `requirements.txt`). A version of Python ≤ 3.11 is required.
- [Jupyter Notebook](https://jupyter.org/install): interactive computing platform accessible from the web browser to run Python code.

## Dataset acquisition

For the first step of this tutorial, the Nucleo board (with the expansion board and adapter board mounted on top) and MEMS Studio can be used to collect multiple data logs for each of the activities.

1. Mount the X-NUCLEO-IKS4A1 expansion board on top of the NUCLEO-F401RE board and connect it to a USB port of your computer. Do not plug any sensor adapter board in the DIL24 socket if it hosts a sensor with the same I2C address as the LSM6DSO16IS sensor already available on the expansion board, as a clash of addresses does not allow the firmware to work correctly.

    ![Nucleo setup](images/nucleo_setup.png)

2. Flash the [LSM6DSO16IS_DataLogExtended.bin](https://github.com/STMicroelectronics/x-cube-ispu/blob/main/Projects/NUCLEO-F401RE/Examples/IKS4A1/LSM6DSO16IS_DataLogExtended/Binary/LSM6DSO16IS_DataLogExtended.bin) firmware from the X-CUBE-ISPU package to enable communication between MEMS Studio and the sensor (this can be achieved simply by copying the *.bin* file to the board mass storage).

3. Open MEMS Studio, under `Connect`, select `Serial` as `Communication type` and the serial port the board is connected to as `Communication port`, and then press `Connect`. Then, if not already showing, select `LSM6DSO16IS` under the `Accelerometer sensor` dropdown menu and press `Select`.

    ![MEMS Studio connect](images/mems_studio_connect.jpg)

4. Go to the `Sensor Evaluation` section and select the `Quick Setup` page to configure the sensor (`Accelerometer Full Scale`: 8 g; `Accelerometer Output Data Rate`: 26 Hz), then press the `▶` button in the top left corner to start streaming data from the sensor.

    ![MEMS Studio setup](images/mems_studio_setup.jpg)

5. Go to the `Save to File` page, select a save path for the CSV log in the `data/<id>_<activity>` folder (where `<activity>` corresponds to the desired activity name and `<id>` corresponds to the numeric identifier of the activity used for its classification), select only `Accelerometer` in both `Data` and `Datalog period source` sections.

    ![MEMS Studio datalog](images/mems_studio_datalog.jpg)

6. Use the `Start` / `Stop` buttons to start and stop data collection making sure to acquire data in the most realistic setting with only one activity type per log.

### PAMAP2 dataset

Several CSV logs are already available in the `data` folder and are sufficient to obtain good training results, however, the user is free to add new logs or customize the activities.

These logs were derived from a public HAR dataset (Reiss, Attila. (2012). PAMAP2 Physical Activity Monitoring. UCI Machine Learning Repository. [https://doi.org/10.24432/C5NW2H](https://doi.org/10.24432/C5NW2H)), which was cleaned/pre-processed for the purpose of this tutorial. The following steps have been applied to the original dataset:

- Only keep logs acquired from the wrist-mounted sensor and only from { *lying*, *sitting*, *standing*, *watching_TV*, *walking*, *nordic_walking*, *upstairs*, *downstairs*, *running*, *cycling* } activities.
- Merge { *lying*, *sitting*, *standing*, *watching_TV* } into *stationary* class, and { *walking*, *nordic_walking*, *upstairs*, *downstairs* } into *walking* class.
- Visually inspect data and remove start/stop segments where no tasks are being performed.
- Only keep accelerometer X, Y, and Z axes data and convert it to mg scale.
- Resample data from 100 Hz to 26 Hz using Fourier method.
- Segment CSV logs into multiple logs of 1 minute each.
- Balance dataset by keeping the same number of logs for each class.
- Convert logs to the MEMS Studio CSV format.

## Model Training

Once a decent-sized dataset has been acquired, the training of the model on the collected data is the next step. To make this process as easy as possible, a ready-to-use notebook (`har_tutorial.ipynb`) is provided.

### Steps

By running all cells in this notebook, the following steps are performed:

1. Load data from the CSV logs contained in the `data/<id>_<activity>` folders.
2. Segment data into 2-second windows without overlap.
3. Label windows using the corresponding `<id>` value.
4. Divide dataset into training (63%), validation (7%), and testing (30%) sets.
5. Train a small 1D-CNN model (772 parameters).
6. Quantize model weights from `float` to `int8` representation.

    Note: Performing model quantization may entail trading some accuracy for significantly reducing the model size.

At the end of the procedure, both the original and quantized models and the test set are saved in the `output_ipynb` folder.

The notebook uses Keras and TensorFlow Lite to create, train, and quantize the model, but other frameworks could be used instead (any framework where the model can be exported or converted to ONNX, such as PyTorch, QKeras and Larq for quantization, etc.).

### Customization

To customize the activities recognized by the model, the user can just add/remove activity folders in `data`. For example, for adding a new driving activity, the user needs to create a `data/4_driving` folder where all CSV logs corresponding to that activity will be placed.

### How to run the Jupyter Notebook

To be able to run the provided Jupyter Notebook, please follow these steps:

1. (Optional) Create a Python virtual environment using [venv](https://docs.python.org/3/library/venv.html), [Anaconda](https://docs.conda.io/projects/conda/en/latest/user-guide/install/index.html) / [Miniconda](https://docs.conda.io/en/latest/miniconda.html) or any other similar tool to avoid conflicts with previously installed Python packages.

2. Install the required modules by using the following shell command from the folder of this tutorial:

    ```powershell
    pip install -r requirements.txt
    ```

3. Start the [Jupyter](https://jupyter.org) server by running:

    ```powershell
    jupyter notebook
    ```

    and open in a web browser the URL displayed in your shell.

    Alternatively, a [Visual Studio Code extension](https://marketplace.visualstudio.com/items?itemName=ms-toolsai.jupyter) is also available for Jupyter integration in Visual Studio Code, which will take care of starting the Jupyter server in the background.

4. Run all the Jupyter Notebook cells.

## ST Edge AI Core command line usage

Having successfully trained a model, ST Edge AI Core can now be used to integrate it inside the ISPU and inspect its behavior for developing an accurate and reliable application.

### Analyze

To assist in deciding which model is more suited for ISPU integration, the `analyze` command can be used to obtain useful information regarding its memory footprint and the number of operations.

By running the following command the Keras (.h5) model can be analyzed:

```powershell
stedgeai analyze --target ispu --model output_ipynb/cnn_8x8x8.h5 --no-workspace
```

Here is the summary from the `st_ai_output/network_analyze_report.txt` report:

```
Exec/report summary (analyze)
----------------------------------------------------------------------------------------------
model file         :   /st/st-mems-ispu/tutorials/st_edge_ai_keras/output_ipynb/cnn_8x8x8.h5
type               :   keras
c_name             :   network
options            :   allocate-inputs, allocate-outputs, use-lite-runtime, use-st-ai
optimization       :   balanced
target/series      :   ispu
workspace dir      :   /tmp/stedgeai_workspace
output dir         :   /st/st-mems-ispu/tutorials/st_edge_ai_keras/st_ai_output
model_fmt          :   float
model_name         :   cnn_8x8x8
model_hash         :   0x97a8c5a35de42c0ce016549bd648a454
params #           :   724 items (2.83 KiB)
----------------------------------------------------------------------------------------------
input 1/1          :   'input', f32(1x52x3), 624 Bytes, activations
output 1/1         :   'output', f32(1x4), 16 Bytes, activations
macc               :   12,960
weights (ro)       :   2,704 B (2.64 KiB) (1 segment) / -192(-6.6%) vs float model
activations (rw)   :   1,780 B (1.74 KiB) (1 segment) *
ram (total)        :   1,780 B (1.74 KiB) = 1,780 + 0 + 0
----------------------------------------------------------------------------------------------
(*) 'input'/'output' buffers can be used from the activations buffer
```

If the ISPU-Toolchain is detected on the system PATH, ST Edge AI Core will use it to estimate the memory footprint of the converted model taking into accout also its code size:

```
  Summary - "ispu" target
  -----------------------------------------------------------
               Code RAM (ro)      %*   Data RAM (rw)       %
  -----------------------------------------------------------
  RT total            10,724   79.9%             872   32.9%
  -----------------------------------------------------------
  TOTAL               13,428                   2,652
  -----------------------------------------------------------
  *  rt/total
```

Alternatively, for the quantized (.tflite) model, run the following command:

```powershell
stedgeai analyze --target ispu --model output_ipynb/qcnn_8x8x8.tflite --no-workspace
```

Here is the summary from the `st_ai_output/network_analyze_report.txt` report:

```
Exec/report summary (analyze)
---------------------------------------------------------------------------------------------------
model file         :   /st/st-mems-ispu/tutorials/st_edge_ai_keras/output_ipynb/qcnn_8x8x8.tflite
type               :   tflite
c_name             :   network
options            :   allocate-inputs, allocate-outputs, use-lite-runtime, use-st-ai
optimization       :   balanced
target/series      :   ispu
workspace dir      :   /tmp/stedgeai_workspace
output dir         :   /st/st-mems-ispu/tutorials/st_edge_ai_keras/st_ai_output
model_fmt          :   ss/sa per channel
model_name         :   qcnn_8x8x8
model_hash         :   0x2f13331f63620603b46a920e27d68f54
params #           :   676 items (760 B)
---------------------------------------------------------------------------------------------------
input 1/1          :   'serving_default_input0', f32(1x52x3), 624 Bytes, activations
output 1/1         :   'conversion_21', f32(1x4), 16 Bytes, activations
macc               :   12,552
weights (ro)       :   760 B (760 B) (1 segment) / -1,944(-71.9%) vs float model
activations (rw)   :   1,008 B (1008 B) (1 segment) *
ram (total)        :   1,008 B (1008 B) = 1,008 + 0 + 0
---------------------------------------------------------------------------------------------------
(*) 'input'/'output' buffers can be used from the activations buffer
```

```
  Summary - "ispu" target
  ----------------------------------------------------------
               Code RAM (ro)      %*   Data RAM (rw)      %
  ----------------------------------------------------------
  RT total            14,172   94.9%               4   0.4%
  ----------------------------------------------------------
  TOTAL               14,932                   1,012
  ----------------------------------------------------------
  *  rt/total
```

Comparing the reports before and after the quantization:

- `macc`: 12960 → 12552
- `weights`: 2704 B → 760 B
- `code ram (ro)`: 13428 B → 14932 B
- `data ram (rw)`: 2652 B → 1012 B

After quantization, the network has about the same number of *macc* operations, and *weights* now occupy ~72% less memory. However, *code ram (ro)*, where *weights* are stored, is even larger than before; this is due to code overhead (needed to handle inference computations in the quantized network) being too much for such a small network. A significant reduction, instead, can be observed for *data ram (rw)*, which is 62% less occupied.

Note: The larger the model the less impact the code overhead will have on the final code size; for this reason, it is always recommended to run the `analyze` command on the model before proceding further in the development process with ST Edge AI Core.

Given the results obtained from the comparison of the model before and after the quantization, the next steps in the tutorial will focus only on the original float model. In this case, the quantized model would be a good choice only if the application had constraints on the size in data RAM.

### Generate

The actual conversion step is performed using the `generate` command, which enables the user to easily generate a C library optimized for the ISPU architecture from the trained model.

By running the following command, the Keras (.h5) model can be converted:

```powershell
stedgeai generate --target ispu --model output_ipynb/cnn_8x8x8.h5 --output generated --no-workspace --no-report
```

The result of this operation is the creation of C files (.c/.h), containing model-specific code and data, and a C runtime library (.h/.a), that make the model inference possible on the ISPU, inside the specified output folder `generated`.

### Validate

To evaluate the model performance and the correctness of the conversion performed by ST Edge AI Core, the `validate` command can be used.

The command offers various functionalities but, in its basic form, by inputting the model with no extra arguments, the tool will generate random data to be used as input to both the original and converted model to check that predictions coincide; alternatively, the user can provide the input / output data directly to ensure more control over the frequencies of the predicted classes.

Another useful option is the ability to perform the validation on target to check the correctness of the converted model on the final target (LSM6DSO16IS) and obtain a measurement of its execution time. In order to do that, first of all, the Nucleo board must be flashed with the [nucleo_f401re_ispu_stedgeai_validate.bin](../../host_firmware/nucleo_ispu_stedgeai_validate/binary/nucleo_f401re_ispu_stedgeai_validate.bin") firmware. Then, the generated C model must be copied inside the [template_stedgeai_validate](https://github.com/STMicroelectronics/ispu-examples/tree/master/ism330is_lsm6dso16is/template_stedgeai_validate) project that must be compiled to create a sensor configuration (.ucf) file:

1. First, copy the `template_stedgeai_validate` project available in the examples folder of this repository:

    ```powershell
    cp -r ../../examples/ism330is_lsm6dso16is/template_stedgeai_validate/ispu ispu_validation
    ```

2. Then copy the content of `generated` inside the template project:

    ```powershell
    cp -r generated/* ispu_validation
    ```

3. Lastly, compile the project using `make` to generate the sensor configuration (.ucf) file and copy it to the `output` folder:

    ```powershell
    make -C ispu_validation/make
    cp ispu_validation/make/bin/ispu.ucf output/har_validate.ucf
    ```

    Note: Make sure you have the ISPU-Toolchain correctly set up on your system to be able to compile the code for the ISPU architecture.

    In alternative to using `make`, the project can be compiled using the Eclipse project pre-configured in the `ispu_validation/eclipse` folder.

Finally, having completed all the previous steps, it is possible to perform the validation on target by connecting the Nucleo board to a USB port of the PC, and running the `validate` command specifying the model (.h5), the sensor configuration (.ucf), and the validation data (.npz) as arguments:

```powershell
stedgeai validate --target ispu --mode target --model output_ipynb/cnn_8x8x8.h5 --valinput output_ipynb/har_testset.npz --ucf output/har_validate.ucf --no-workspace
```

The `st_ai_output/network_validate_report.txt` report includes information about the execution time and model accuracy:

```
ST.AI Profiling results v2.0 - "network"
---------------------------------------------------------------
nb sample(s)   :   527
duration       :   69.380 ms by sample (69.213/69.661/0.113)
macc           :   12960
cycles/MACC    :   26.77
---------------------------------------------------------------
```

The most important piece of information here is given by the `duration` field, which reports that the model inference is taking ~69 ms for its execution; this is an important parameter to know for running the model in real time.

```
Evaluation report (summary)
-----------------------------------------------------------------------------------------------------------------------------------------------------------
Output              acc       rmse          mae           l2r           mean        std        nse        cos        tensor
-----------------------------------------------------------------------------------------------------------------------------------------------------------
TARGET c-model #1   98.29%    0.080867730   0.011736452   0.163443178   0.000000    0.080887   0.965139   0.986838   'output', 527 x f32(1x4), m_id=[16]
original model #1   98.29%    0.080867730   0.011736450   0.163443163   -0.000000   0.080887   0.965139   0.986838   'output', 527 x f32(1x4), m_id=[16]
X-cross #1          100.00%   0.000000041   0.000000012   0.000000082   0.000000    0.000000   1.000000   1.000000   'output', 527 x f32(1x4), m_id=[16]
-----------------------------------------------------------------------------------------------------------------------------------------------------------
```

In the evaluation report, three rows of results are reported, their meaning is the following:

- TARGET c-model: performance of the converted model using given outputs as ground-truth.
- original model: performance of the original model using given outputs as ground-truth.
- X-cross: performance of the converted model using the original model outputs as ground-truth.

In this case, the converted model performance is practically the same as the original model (for more details on validation metrics refer to the "Evaluation metrics" article of the documentation available within the ST Edge AI Core installation folder).

## ST Edge AI integration in MEMS Studio

To accomodate users that prefer to use a graphical interface over the command line, ST Edge AI Core has been integrated in MEMS Studio.

After opening MEMS Studio, go to the `Advanced Features` section and select the `ISPU Model Converter` page. In the lower portion of the page are located four buttons that open up subpages corresponding to ST Edge AI Core's main functionalities:

- Load NN model / Generate: generate the converted model for the ISPU architecture starting from the original model.

    ![MEMS Studio generate](images/mems_studio_stedgeai_generate.jpg)

- Analyze: obtain useful information regarding the model's memory footprint and complexity.

    ![MEMS Studio analyze](images/mems_studio_stedgeai_analyze.jpg)

- Validate: evaluate the model performance and the correctness of the conversion performed by ST Edge AI Core.

    - On host: run the validation on the user's computer.

        ![MEMS Studio validate on host](images/mems_studio_stedgeai_validate_host.jpg)

    - On target: run the validation directly on the ISPU (note: requires the Nucleo board to be flashed with the [nucleo_f401re_ispu_stedgeai_validate.bin](../../host_firmware/nucleo_ispu_stedgeai_validate/binary/nucleo_f401re_ispu_stedgeai_validate.bin") firmware).

        ![MEMS Studio validate on target](images/mems_studio_stedgeai_validate_target.jpg)

- Benchmark: assess all benchmark results (original model, c-model, and X-cross) from both host and target validations.

    ![MEMS Studio benchmark](images/mems_studio_stedgeai_benchmark.jpg)

## ISPU firmware integration

After the validation phase, if the results are satisfactory, the next step is the integration inside the ISPU firmware and the implementation of the actual logic of the application.

The first step is to copy the [template_stedgeai](https://github.com/STMicroelectronics/ispu-examples/tree/master/ism330is_lsm6dso16is/template_stedgeai) project and add the C code generated from the model:

```powershell
cp -r ../../examples/ism330is_lsm6dso16is/template_stedgeai/ispu ispu_integration
cp -r generated/* ispu_integration
```

Then, for the actual integration, the `main.c` template must be modified to do the following steps:

1. Read accelerometer data and store it inside a buffer of length `52 samples x 3 axes`.
2. Once the buffer is full, run the model inference to obtain the prediction.
3. Write the model prediction in the ISPU output registers.

### Add definitions and global variables

The integration template is agnostic of the specific application, therefore, as a first step, all relevant information, such as the accelerometer sensitivity, must be added.

```c
#define ACC_SENS 0.244f // [mg/LSB]
```

Next, the variables to implement the application logic are needed. In this case: an array of label strings, and the number of samples currently stored inside the input buffer.

```c
static const char *labels[] = { "stationary", "walking", "running", "cycling" };
static uint8_t win_cnt;
```

### Initialize algorithm state

To ensure correct functioning, the initialization logic must be placed inside the `algo_00_init` function.

```c
void __attribute__ ((signal)) algo_00_init(void)
{
	(void)stai_runtime_init();
	(void)stai_network_init(net); // initialize the network context

	init_network_buffers(net, input_buffers, output_buffers);

	// initialize state variables
	win_cnt = 0;
}
```

Compared to the template, the initialization of the `win_cnt` variable was added.

### Fill model input buffer

After initialization, all the logic of the application, including running the model, should be placed in the `algo_00` function that is called every time new data is ready to be read.

```c
void __attribute__ ((signal)) algo_00(void)
{
	// ispu output registers base address
	void *out_addr = (void *)ISPU_DOUT_00;

	// reinterpret input buffer as a multi-dimensional array of shape {1,52,3}
	float (*input)[STAI_NETWORK_IN_1_HEIGHT][STAI_NETWORK_IN_1_CHANNEL] =
		(float (*)[STAI_NETWORK_IN_1_HEIGHT][STAI_NETWORK_IN_1_CHANNEL])input_buffers[0];

	// reinterpret output buffer as a multi-dimensional array of shape {1,4}
	float (*output)[STAI_NETWORK_OUT_1_CHANNEL] =
		(float (*)[STAI_NETWORK_OUT_1_CHANNEL])output_buffers[0];
```

Here we declare three variables:
- `out_addr`: address of the first ISPU output register, used as a starting address to write values to the output registers and thus share them with the host microcontroller or microprocessor.
- `input`: pointer to the model input buffer where new accelerometer samples will be stored.
- `output`: pointer to the model output buffer where new model predictions will be stored.

```c
	// read accelerometer data and place it inside input buffer
	input[0][win_cnt][0] = cast_sint16_t(ISPU_ARAW_X) * ACC_SENS;
	input[0][win_cnt][1] = cast_sint16_t(ISPU_ARAW_Y) * ACC_SENS;
	input[0][win_cnt][2] = cast_sint16_t(ISPU_ARAW_Z) * ACC_SENS;
```

Here new accelerometer data is converted from LSB to mg unit (this is important since the model has been trained on data in mg unit) and placed inside the model input buffer.

```c
	// write accelerometer data to output registers
	for (uint8_t i = 0; i < STAI_NETWORK_IN_1_CHANNEL; i++, out_addr += sizeof(float))
		cast_float(out_addr) = input[0][win_cnt][i];
```

Additionally, the accelerometer data is written to the ISPU output registers for debugging purposes.

### Run model inference

After incrementing the samples counter, if the number of samples has reached the window length, the model inference can be run.

```c
	// increment count and check if input buffer is ready
	if (++win_cnt == STAI_NETWORK_IN_1_HEIGHT) {
		win_cnt = 0;

		// run model inference
		stai_network_run(net, STAI_MODE_SYNC);

		// prediction corresponds to the output with the highest probability
		uint8_t prediction = 0;
		float max_prob = -1.0f;
		for (uint8_t i = 0; i < STAI_NETWORK_OUT_1_CHANNEL; i++) {
			if (output[0][i] > max_prob) {
				max_prob = output[0][i];
				prediction = i;
			}
		}
	}
```

The function `stai_network_run` will do all the work by forwarding the input buffer through all layers of the network and updating the output buffer.

To get the actual prediction from the softmax output layer, some logic must be implemented to obtain the index of the output value associated with the highest probability.

```c
	// write prediction results to output registers
	for (uint8_t i = 0; i < STAI_NETWORK_OUT_1_CHANNEL; i++, out_addr += sizeof(float))
		cast_float(out_addr) = output[0][i];
	strcpy((char *)out_addr, labels[prediction]);
```

Finally, the prediction probabilities and predicted class label can be written to the ISPU output registers.

### Metadata

In order to be able to correctly visualize the outputs with STMicroelectronics tools, the `meta.txt` file from the template must be populated with a set of lines describing the outputs format. A pre-made output format file is already available for the model of this tutorial in the `ispu` folder:

```
description "Human activity recognition"

output "Acc x [mg]" float
output "Acc y [mg]" float
output "Acc z [mg]" float
output "Stationary" float
output "Walking" float
output "Running" float
output "Cycling" float
output "Prediction" char 10
```

The above metadata defines:
- The description of the ISPU program as "Human activity recognition".
- The 3 accelerometers axes in float format as the first outputs in the ISPU_DOUT registers.
- The probability, in float format, of the 4 classes being detected (raw outputs of the network) as the next outputs.
- The label corresponding to the predicted class as a string of 10 characters as last output.

For details on the syntax to use for this file refer to [README](https://github.com/STMicroelectronics/ispu-examples/tree/master/ism330is_lsm6dso16is/template/README.md) of the generic template for the LSM6DSO16IS device and to the help of the `ispu_gen` tool (part of the ISPU-Toolchain).

For instructions on how to integrate a generic model in the template, refer to the [README](https://github.com/STMicroelectronics/ispu-examples/tree/master/ism330is_lsm6dso16is/template_stedgeai/README.md) of the template itself.

### Generate the sensor configuration

Once the changes to `main.c` and `meta.txt` have been implemented, the project can be compiled to generate the sensor configuration (.json/.h):

```powershell
make -C ispu_integration/make
cp ispu_integration/make/bin/ispu.json output/har_tutorial.json
cp ispu_integration/make/bin/ispu.h output/har_tutorial.h
```

In alternative to using `make`, the project can be compiled using the Eclipse project pre-configured in the `ispu_integration/eclipse` folder.

For convenience, a reference ISPU project integrating the HAR model is already provided in the `ispu` folder and the prebuilt sensor configuration files can be found in `output`.

## Testing in real-time

MEMS Studio can then be used to upload and test the sensor configuration (.json) containing the ISPU program:

1. Make sure the Nucleo board has been flashed using the [LSM6DSO16IS_DataLogExtended.bin](https://github.com/STMicroelectronics/x-cube-ispu/blob/main/Projects/NUCLEO-F401RE/Examples/IKS4A1/LSM6DSO16IS_DataLogExtended/Binary/LSM6DSO16IS_DataLogExtended.bin) firmware (the same firmware used for data logging).

2. Connect the board, go to the `Sensor Evaluation` section, and then select the `Quick Setup` page.

3. Click on the `Load configuration file` button to open a file dialog window and select the `har_tutorial.json` file to load the configuration:

    ![MEMS Studio configuration loading](images/mems_studio_load_config.jpg)

4. Once the upload is completed, go to the `Data Monitor` page and press the `▶` button in the top left corner to start reading the `ISPU Output` results from the ISPU program:

    ![MEMS Studio data monitoring](images/mems_studio_outputs.jpg)

Alternatively, the [ISPU test firmware](../../host_firmware/nucleo_ispu_test_header/nucleo-f401re) available in this repository can be used to automatically build a dedicated firmware for testing the model.

For detailed instructions on all the ways to build and flash the firmware, refer to its [README](../../host_firmware/nucleo_ispu_test_header/README.md). If the Arm GNU Toolchain is installed and added to the PATH, the firmware can be built as follows:
```powershell
make -C ../../host_firmware/nucleo_ispu_test_header/nucleo-f401re ISPU_DIR=../../../tutorials/st_edge_ai_keras/ispu_integration
```

In order to flash the firmware, the flash scripts included in the firmware folder can be used, or the built binary can be copied to the Nucleo board mass storage as usual.

Once the firmware has been flashed, the outputs are printed directly on the UART interface of the Nucleo board, which can be read with any terminal emulator with serial communication support. The output will look as follows:

```
Acc x [mg]	Acc y [mg]	Acc z [mg]	Stationary	Walking		Running		Cycling		Prediction
-358.436005	172.020004	958.432007	0.000000	0.000000	0.000000	0.000000	stationary
-384.056000	298.656006	750.299988	0.000000	0.000000	0.000000	0.000000	stationary
-294.752014	307.928009	556.075989	0.000000	0.000000	0.000000	0.000000	stationary
-326.227997	228.139999	343.795990	0.000000	0.000000	0.000000	0.000000	stationary
-445.787994	177.143997	283.283997	0.000000	0.000000	0.000000	0.000000	stationary
-46.360001	-18.788000	161.772003	0.000000	0.000000	0.000000	0.000000	stationary
-55.875999	118.828003	221.063995	0.000000	0.000000	0.000000	0.000000	stationary
-989.419983	805.687988	672.219971	0.000000	0.000000	0.000000	0.000000	stationary
...
-834.968018	805.687988	1391.776001	0.000105	0.986666	0.012337	0.000892	walking
-491.660004	360.388000	704.671997	0.000105	0.986666	0.012337	0.000892	walking
-547.536011	101.015999	233.264008	0.000105	0.986666	0.012337	0.000892	walking
-407.235992	-57.096001	-0.976000	0.000105	0.986666	0.012337	0.000892	walking
-107.848000	-183.000000	-287.432007	0.000105	0.986666	0.012337	0.000892	walking
-56.852001	134.688004	-22.204000	0.000105	0.986666	0.012337	0.000892	walking
-443.592010	518.987976	689.788025	0.000105	0.986666	0.012337	0.000892	walking
-583.892029	618.539978	1338.583984	0.000105	0.986666	0.012337	0.000892	walking

```

For more information on how to use this firmware, refer to its [README](../../host_firmware/nucleo_ispu_test_header/README.md).

------

**More information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2024 STMicroelectronics**
