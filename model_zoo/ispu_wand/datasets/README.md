# ISPU wand datasets

In this folder, you can put your datalogs to quickly implement your personal use case.

The *lsm6dso16is_ISPU_reference_dataset* folder contains the dataset used to build the pretrained example model available at [../models/lsm6dso16is_ISPU_reference_dataset](../models/lsm6dso16is_ISPU_reference_dataset) and in the [ST Edge AI Developer Cloud](https://www.st.com/en/development-tools/stedgeai-dc.html).

## Dataset format

The datalogs must be in CSV format with the three columns `acc_x[mg]`,
`acc_y[mg]`, `acc_z[mg]` separated by commas. This is the log format of the ST MEMS evaluation software [MEMS Studio](https://www.st.com/en/development-tools/mems-studio.html) when logging accelerometer data only in mg.

Each CSV file must contain only **different repetitions of the same air-written letter**. It is important to move slowly between repetitions to ensure they are correctly segmented by the training scripts. More information about gesture segmentation, both during training and in the final ISPU code, is available in the main [README](../README.md) and in the training [README](../training/README.md).

Formally, a dataset is a directory with the following structure:
- dataset root folder (level 0)
	- user folder (level 1)
		- air-written letter folder (level 2)
			- multiple logs of the air-written letter (level 3)

**Note**: The folders at level 2 must be named as *n_x* where *n* is the numeric identifier of the target letter *x* (that is, the encoding that will be used by the model to identify that particular letter). The mapping between the target air-written letter and numerical identifier must match across all users in the same dataset for the application to work properly. There are no naming conventions enforced for folders at level 0, 1, and 3.

For a practical example of a dataset, the [lsm6dso16is_ISPU_reference_dataset](./lsm6dso16is_ISPU_reference_dataset) folder structure and datalogs format can be consulted.

------

**More information: [http://www.st.com](http://st.com/MEMS)**

**Copyright © 2024 STMicroelectronics**
