##############################################################################
# Copyright(c) 2024 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
##############################################################################

import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

from keras import Model
from keras.callbacks import History
from sklearn.metrics import confusion_matrix
from typing import List, Optional


def plot_gestures(df, ax, indexes, fs, win_len) -> None:
    """Show data frame segmentation

    Args:
        df (pd.DataFrame): Unsegmented dataframe.
        ax (matplotlib.axes._axes.Axes): Matplotlib ax (returned from plt.subplots).
        indexes (List[int]): List of segmentation indexes
        fs (int): Application sample frequency (i.e., sensor ODR)
        win_len (int): Window sample length.
    """
    time_ax = [i / fs for i in range(len(df))]
    ax.plot(time_ax, df)
    ax.set_xlabel('Time [s]')
    ax.set_ylabel('Acceleration [g]')
    ax.legend(['acc_x[mg]', 'acc_y[mg]', 'acc_z[mg]'])
    for i in indexes:
        ax.axvspan(i / fs, (i + win_len) / fs, color=(1, 0, 0, 0.2))


def plot_model_history(model: Model, history: History) -> None:
    """Plot model training history.

    Args:
        model (Model): Keras model instance.
        history (History): Training history instance.
    """
    _, ax_list = plt.subplots(1, 2, figsize=(12, 4), sharex=True)
    for metric, ax in zip(["accuracy", "loss"], ax_list):
        ax.set_title(f"{model.name} {metric}")
        ax.plot(history.history[f"{metric}"])
        ax.plot(history.history[f"val_{metric}"])
        ax.set_ylabel(f"{metric}")
        ax.set_xlabel("epoch")
        ax.legend(["train", "validation"], loc="upper left")


def plot_confusion_matrix(
    y_test: np.ndarray,
    y_pred: np.ndarray,
    labels: List[str],
    title: Optional[str] = None
) -> None:
    """Plot confusion matrix with given class names.

    Args:
        y_test (np.ndarray): True labels.
        y_pred (np.ndarray): Predicted labels.
        labels (List[str]): Class names.
        title (str | None): Plot figure title. Defaults to None.
    """
    _, ax = plt.subplots(1, 1, figsize=(6, 5))
    ax = plt.subplot(1, 1, 1)
    if title is None:
        title = "Confusion Matrix"
    ax.set_title(title)
    cm = confusion_matrix(y_test, y_pred, normalize="true")
    df_cm = pd.DataFrame(cm, labels, labels)
    sns.heatmap(df_cm, ax=ax, annot=True, annot_kws={"size": 12}, fmt=".2f")
