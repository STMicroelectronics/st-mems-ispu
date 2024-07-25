##############################################################################
# Copyright(c) 2024 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
##############################################################################

import os
import sys
import pathlib
import random
import shutil
import logging
import yaml

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'  # nopep8

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import tensorflow as tf

from scipy.signal import lfilter, butter
from sklearn.model_selection import train_test_split

from keras.utils import to_categorical
from keras.callbacks import EarlyStopping, ModelCheckpoint

from schema import SchemaError

from preprocess import get_header_line, segment_gestures, yaml2header, yaml2json, yaml2conf, fs2sens
from preprocess import ISPU_WAND_CONF_SCHEMA
from ispu_wand_model import CNN_model
from plot import plot_gestures, plot_model_history, plot_confusion_matrix


def main():

    if len(sys.argv) < 2:
        print('Usage: python train.py <path/to/user_config.yaml>')
        sys.exit(1)

    base_path = pathlib.Path(__file__).parent
    user_config_file = sys.argv[1]
    with open(base_path.parent / user_config_file) as f:
        user_config = yaml.full_load(f)

    try:
        user_config = ISPU_WAND_CONF_SCHEMA.validate(user_config)
    except SchemaError as e:
        sys.exit(f'Configuration error: {e.code}')

    checkpoint_model_dir = base_path / 'models'
    if os.path.exists(checkpoint_model_dir):
        shutil.rmtree(checkpoint_model_dir)
    os.mkdir(checkpoint_model_dir)

    out_dir = base_path.parent / 'models' / \
        user_config['general']['name'] / 'training_output'

    if os.path.exists(out_dir):
        shutil.rmtree(out_dir)
    os.makedirs(out_dir)

    SEED = 42
    random.seed(SEED)
    np.random.seed(SEED)
    tf.random.set_seed(SEED)
    COL_ALIASES = {
        'acc_x[mg]': 'ax',
        'acc_y[mg]': 'ay',
        'acc_z[mg]': 'az',
    }
    KEEP_COLUMNS = ['ax', 'ay', 'az']
    lp_cut = user_config['preprocessing']['lp_cut']
    btype = 'bandpass' if lp_cut is not None else 'highpass'
    Wn = [user_config['preprocessing']['hp_cut'], user_config['preprocessing']['lp_cut']] if lp_cut is not None \
        else user_config['preprocessing']['hp_cut']
    order = 1 if btype == 'bandpass' else 2

    odr = user_config['preprocessing']['f_sample']
    win_ths = user_config['preprocessing']['win_ths_g']
    win_len = user_config['preprocessing']['win_len_samples']
    trig_len = user_config['preprocessing']['trig_len_samples']
    reset_len = user_config['preprocessing']['reset_len_samples']
    discard_len = user_config['preprocessing']['discard_len_samples']
    dataset_name = user_config['dataset']['path']

    IIR2_B, IIR2_A = butter(
        fs=odr, N=order, Wn=Wn, btype=btype
    )
    X = []
    y = []

    user_config['labels'] = dict()
    print(f'Starting gestures segmentation of {dataset_name} dataset')
    for log_path in pathlib.Path(dataset_name).glob('**/*.csv'):
        label, letter = log_path.parent.name.split('_')
        user_config['labels'][label] = letter.upper()
        # Read CSV data
        df = pd.read_csv(log_path, sep=',', header=get_header_line(log_path), dtype=np.float32).rename(
            columns=COL_ALIASES)[KEEP_COLUMNS]
        # Convert mg to g
        df /= 1000
        df_orig = df.copy()
        # Apply bandpass filter
        df.values[:] = lfilter(IIR2_B, IIR2_A, df, axis=0)
        # Segment data into windows containing the gesture
        win_list, indexes = segment_gestures(
            df,
            odr,
            win_ths,
            win_len,
            trig_len,
            reset_len,
            discard_len
        )
        show_plot = user_config['preprocessing']['show_segmentation']
        # Plot data to debug segmentation
        if show_plot is not None:
            fig, ax = plt.subplots()

            log_name = list(log_path.parents[0:3][::-1]) + [log_path]
            log_name = '/'.join([log.stem if log.name ==
                                ' ' else log.name for log in log_name])
            ax.set_title(f'{log_name} {show_plot}')

            if show_plot == 'raw':
                plot_gestures(df_orig, ax, indexes, odr, win_len)
            else:
                plot_gestures(df, ax, indexes, odr, win_len)
            plt.show()
        # Append features and labels
        X += win_list
        y += [int(label)] * len(win_list)

    X = np.array(X)
    y = to_categorical(y)
    print('Class\tSamples')
    print('---------------')
    labels = sorted(user_config['labels'].items(), key=lambda el: el[0])
    labels = [el[1] for el in labels]

    for i in range(y.shape[1]):
        print(f'{labels[i]}:\t{len(np.argwhere(y[:, i] == 1))}')
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, stratify=y, train_size=user_config['training']['train_test_split'])
    X_train, X_valid, y_train, y_valid = train_test_split(
        X_train, y_train, stratify=y_train, train_size=1.0 - user_config['training']['train_validation_split'])

    print(f'Training set:   {len(y_train):>4} samples')
    print(f'Validation set: {len(y_valid):>4} samples')
    print(f'Testing set:    {len(y_test):>4} samples')
    model = CNN_model(
        inp_shape=X_train.shape[1:],
        out_shape=y_train.shape[1],
        hidden=3,
        filters=8,
        kernel_size=3,
        pool_size=2,
        dropout=0.2,
        learning_rate=1e-3
    )
    model_path = str(checkpoint_model_dir / f'{model.name}.h5')

    callbacks = [
        EarlyStopping(
            monitor='val_loss',
            verbose=1,
            patience=20,
            restore_best_weights=True
        ),
        ModelCheckpoint(
            filepath=model_path,
            patience=20,
            monitor='val_loss',
            save_best_only=True,
            save_freq='epoch',
        )
    ]

    history = model.fit(
        X_train, y_train,
        validation_data=(X_valid, y_valid),
        epochs=user_config['training']['epochs'],
        batch_size=user_config['training']['batch_size'],
        callbacks=callbacks
    )

    model.load_weights(model_path)
    model.save(str(out_dir / f'{model.name}.h5'))
    np.savez(os.path.join(out_dir, 'validation_data.npz'),
             m_inputs=X_test, m_outputs=y_test)

    sens = fs2sens(user_config['preprocessing']['full_scale_g'])
    user_config['preprocessing']['acc_sens_g'] = sens

    yaml2header(out_dir, user_config, IIR2_A, IIR2_B)
    yaml2json(out_dir, user_config)
    yaml2conf(out_dir, user_config)

    loss, acc = model.evaluate(X_test, y_test)

    y_pred = model.predict(X_test)
    y_test_arg = np.argmax(y_test, axis=1)
    y_pred_arg = np.argmax(y_pred, axis=1)

    if user_config['training']['show_stats']:
        plot_model_history(model, history)
        plot_confusion_matrix(
            y_test_arg, y_pred_arg, labels, f'{model.name} with no threshold'
        )
        plt.show()

        model.summary()
        print(f'\nPrediction accuracy: {acc:.02%}')


if __name__ == '__main__':
    main()
