##############################################################################
# Copyright(c) 2024 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
##############################################################################

from keras import Model
from keras.layers import Input, Conv1D, BatchNormalization, MaxPooling1D, \
    Flatten, Dense, Dropout, Softmax, Activation
from keras.optimizers import Adam
from typing import Optional, Tuple


def CNN_model(
    inp_shape: Tuple[int, int],
    out_shape: int,
    hidden: int,
    filters: int,
    kernel_size: int,
    pool_size: int,
    dropout: Optional[float] = None,
    learning_rate: float = 1e-3
) -> Model:
    """Create Convolutional Neural Network (CNN) using 1D layers.

    Args:
        inp_shape (Tuple[int, int]): Input shape (#window_length, #channels).
        out_shape (int): Output shape (#classes).
        hidden (int): Number of hidden layers.
        filters (int): Number of convolutional filters in each hidden layer.
        kernel_size (int): Size of kernel in each convolutional layer.
        pool_size (int): Pool size of max pooling layer.
        dropout (float | None): Dropout rate in final dense layer.
            Defaults to None.
        learning_rate (float): Adam optimizer learning rate. Defaults to 1e-3.

    Returns:
        Model: 1D convolutional Neural Network.
    """
    x = Input(shape=inp_shape, name="input")
    model_input = x
    for i in range(hidden):
        x = Conv1D(
            filters=filters,
            kernel_size=kernel_size,
            padding="same",
            activation=None,
            kernel_regularizer="l2",
            name=f"conv_hidden_{i}",
        )(x)
        x = BatchNormalization(scale=True)(x)
        x = Activation('relu')(x)
        x = MaxPooling1D(pool_size=pool_size)(x)
    x = Flatten()(x)
    x = Dense(
        units=out_shape,
        activation=None,
        kernel_regularizer="l2",
        name="dense_out"
    )(x)
    if dropout is not None:
        x = Dropout(rate=dropout)(x)
    model_output = Softmax(name="output")(x)
    model = Model(
        model_input,
        model_output,
        name="cnn_" + "x".join([str(filters)] * hidden)
    )
    model.compile(
        optimizer=Adam(learning_rate),
        loss="categorical_crossentropy",
        metrics=["accuracy"]
    )
    return model
