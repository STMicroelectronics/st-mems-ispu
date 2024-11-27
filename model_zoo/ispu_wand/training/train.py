##############################################################################
# Copyright(c) 2024 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
##############################################################################

from typing import List, Tuple, Dict, Union, Optional
import os
import sys
import pathlib
import random
import shutil

import yaml
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import tensorflow as tf

from scipy.signal import lfilter, butter
from sklearn.model_selection import train_test_split
from keras.utils import to_categorical
from keras.callbacks import EarlyStopping, ModelCheckpoint
from schema import SchemaError

from scripts.preprocess import segment_gestures
from scripts.model import get_cnn1d_model
from scripts.plot import plot_gestures, plot_model_history, plot_confusion_matrix
from scripts.utils import (
    get_sensitivity,
    get_csv_header,
    yaml_to_header,
    yaml_to_json,
    yaml_to_conf,
    validate_user_config_yaml
)


def main() -> None:
    if len(sys.argv) < 2:
        print("Usage: python train.py <path/to/user_config.yaml>")
        sys.exit(1)

    base_path = pathlib.Path(__file__).parent
    user_config_file = pathlib.Path(sys.argv[1])
    with open(user_config_file, encoding="utf-8") as f:
        user_config = yaml.full_load(f)

    try:
        user_config = validate_user_config_yaml(user_config)
    except SchemaError as e:
        sys.exit(f"Configuration error: {e.code}")

    checkpoint_model_dir = base_path / "models"
    if os.path.exists(checkpoint_model_dir):
        shutil.rmtree(checkpoint_model_dir)
    os.mkdir(checkpoint_model_dir)

    out_dir = (
        base_path.parent /
        "models" /
        user_config["general"]["name"] /
        "training_output"
    )

    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    dataset_name = user_config["dataset"]["path"]
    odr = user_config["preprocessing"]["f_sample"]

    # Filter parameters
    hp_cut: float = user_config["preprocessing"]["hp_cut"]
    lp_cut: Optional[float] = user_config["preprocessing"]["lp_cut"]
    bp_cut: Union[float, Tuple[float, float]] = hp_cut if lp_cut is None else (hp_cut, lp_cut)
    btype = "bandpass" if lp_cut is not None else "highpass"
    order = 1 if btype == "bandpass" else 2
    iir2_b, iir2_a = butter(fs=odr, N=order, Wn=bp_cut, btype=btype)

    # Segmentation parameters
    win_ths = user_config["preprocessing"]["win_ths_g"]
    win_len = user_config["preprocessing"]["win_len_samples"]
    trig_len = user_config["preprocessing"]["trig_len_samples"]
    reset_len = user_config["preprocessing"]["reset_len_samples"]
    discard_len = user_config["preprocessing"]["discard_len_samples"]
    show_plot = user_config["preprocessing"]["show_segmentation"]

    # Set random seed for reproducibility
    random_seed = 42
    random.seed(random_seed)
    np.random.seed(random_seed)
    tf.random.set_seed(random_seed)

    labels_dict: Dict[str, int] = {}
    X_list: List[np.ndarray] = []
    y_list: List[int] = []

    print(f"Starting gestures segmentation of {dataset_name} dataset...")
    for log_path in pathlib.Path(dataset_name).glob("**/*.csv"):
        label_num, label_letter = log_path.parent.name.split("_")
        label_num = int(label_num)
        labels_dict[label_letter.upper()] = label_num
        # Read CSV data
        df = pd.read_csv(
            log_path,
            sep=",",
            header=get_csv_header(str(log_path)),
            dtype=np.float32
        ).rename(columns={
            "acc_x[mg]": "ax",
            "acc_y[mg]": "ay",
            "acc_z[mg]": "az"
        })
        # Convert mg to g
        df /= 1000.0
        df_orig = df.copy()
        # Apply bandpass filter
        df.values[:] = lfilter(iir2_b, iir2_a, df, axis=0)
        # Segment data into windows containing the gesture
        win_list, indexes = segment_gestures(
            df, win_ths, win_len, trig_len, reset_len, discard_len
        )
        show_plot = user_config["preprocessing"]["show_segmentation"]
        # Plot data to debug segmentation
        if show_plot is not None:
            _, ax = plt.subplots()
            ax.set_title(f"{log_path} {show_plot}")
            df_plot = df_orig if show_plot == "raw" else df
            plot_gestures(df_plot, ax, indexes, odr, win_len)
            plt.show()
        # Append features and labels
        X_list += win_list
        y_list += [label_num] * len(win_list)

    X = np.array(X_list)
    y = to_categorical(y_list)

    print("\nClass\tSamples\n---------------")
    min_count = np.inf
    min_label = -1
    class_freq = {}
    for i, label in enumerate(labels_dict.keys()):
        count = len(y[y[:, i] == 1])
        class_freq[label] = count
        if (count < min_count):
            min_count = count
            min_label = i
        print(f"{label}:\t{count}")

    print("\nBalancing...\n")
    keep_idx = np.argwhere(y[:, min_label] == 1)[:, 0]
    for i, _ in enumerate(labels_dict.keys()):
        if i == min_label:
            continue
        idx = np.random.choice(
            np.argwhere(y[:, i] == 1)[:, 0],
            min_count,
            replace=False
        )
        keep_idx = np.append(keep_idx, idx, axis=0)
    X = X[keep_idx]
    y = y[keep_idx]

    print("Class\tSamples\n---------------")
    for i, label in enumerate(labels_dict.keys()):
        print(f"{label}:\t{len(y[y[:, i] == 1])}")

    # Split data into train, test and validation sets
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, stratify=y,
        train_size=user_config["training"]["train_test_split"]
    )
    X_train, X_valid, y_train, y_valid = train_test_split(
        X_train, y_train, stratify=y_train,
        train_size=1.0 - user_config["training"]["train_validation_split"]
    )
    print(
        f"\nTraining set:   {len(y_train):>4} samples\n"
        f"Validation set: {len(y_valid):>4} samples\n"
        f"Testing set:    {len(y_test):>4} samples\n\n"
    )

    # Instantiate and train CNN-1D model
    model = get_cnn1d_model(
        inp_shape=X_train.shape[1:],
        out_shape=y_train.shape[1],
        hidden=3,
        filters=8,
        kernel_size=3,
        pool_size=2,
        dropout=0.2,
        learning_rate=1e-3
    )
    model.summary()

    model_path = str(checkpoint_model_dir / f"{model.name}.h5")
    history = model.fit(
        X_train,
        y_train,
        validation_data=(X_valid, y_valid),
        epochs=user_config["training"]["epochs"],
        batch_size=user_config["training"]["batch_size"],
        callbacks=[
            EarlyStopping(
                monitor="val_loss",
                verbose=1,
                patience=20,
                restore_best_weights=True
            ),
            ModelCheckpoint(
                filepath=model_path,
                patience=20,
                monitor="val_loss",
                save_best_only=True,
                save_freq="epoch"
            )
        ]
    )

    # Evaluate model performance
    _, acc = model.evaluate(X_test, y_test)
    y_pred = model.predict(X_test)
    y_test_arg = np.argmax(y_test, axis=1)
    y_pred_arg = np.argmax(y_pred, axis=1)

    if user_config["training"]["show_stats"]:
        plot_model_history(model, history)
        plot_confusion_matrix(
            y_test_arg,
            y_pred_arg,
            list(labels_dict.keys()),
            f"{model.name} with no threshold"
        )
        plt.show()

    print(f"\nPrediction accuracy: {acc:.02%}")

    model.save(str(out_dir / f"{model.name}.h5"))
    model.load_weights(model_path)
    np.savez(
        os.path.join(out_dir, "validation_data.npz"),
        m_inputs=X_test,
        m_outputs=y_test
    )

    # Add extra fields to user_config for C header file generation
    user_config["preprocessing"]["acc_sens_g"] = get_sensitivity(
        user_config["preprocessing"]["full_scale_g"])
    user_config["labels"] = labels_dict

    # Generate C header, output JSON file, and config file
    yaml_to_header(out_dir, user_config, iir2_a, iir2_b)
    yaml_to_json(out_dir, user_config)
    yaml_to_conf(out_dir, user_config)


if __name__ == "__main__":
    main()
