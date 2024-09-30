##############################################################################
# Copyright(c) 2024 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
##############################################################################

import os
import shutil
import pathlib

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from schema import Schema, Use, And, Or, Optional
from typing import List

"""
Utilities to validate YAML user configurations
"""


def is_positive(n: int):
    return n > 0


def is_non_negative(n: int):
    return n >= 0


def is_between_0_1(x: float):
    return x >= 0 and x <= 1


def is_valid_segmentation_mode(s: str):
    return s is None or s in {'raw', 'filtered'}


__SUPPORTED_ODR = {
    12.5,
    26,
    52,
    104,
    208,
    416,
    833,
    1667,
    3333,
    6667
}

__SUPPORTED_POWER_MODES = {
    'low_power': (1.6, 12.5, 26, 52, 104, 208),
    'high_performance': (12.5, 26, 52, 104, 208, 416, 833, 1667, 3333, 6667)
}

__SUPPORTED_FS = {
    2: 0.000061,
    4: 0.000122,
    8: 0.000244,
    16: 0.000488
}


def ispu_valid_odr(odr: float):
    """Check if the input Output Data Rate is supported by the ispu

    Args:
            odr (float): Output Data Rate setting.
    """
    return odr in __SUPPORTED_ODR


def ispu_valid_full_scale(fs: int):
    """Check if the input full-scale is supported by the ispu

    Args:
            fs (int): Full scale setting.
    """
    return fs in __SUPPORTED_FS


def ispu_valid_powermode(power_mode: str):
    """Check if the input power mode is valid for the ispu

    Args:
        power_mode (str): Power mode setting.
    """

    return power_mode in __SUPPORTED_POWER_MODES


def ispu_supported_powermode(odr: float, power_mode: str):
    """Check if the input power mode is supported by the ispu given a certain odr

    Args:
        odr (float): Output Data Rate setting.
        power_mode (str): Power mode setting.
    """
    return power_mode in _SUPPORTED_POWER_MODES and odr in _SUPPORTED_POWER_MODES[power_mode]


ISPU_WAND_CONF_SCHEMA = Schema(
    {
        'general': {
            'name': Use(str, error='name of the use case must be a string')
        },
        'dataset': {
            'name': Use(str, error='name of the dataset must be a string'),
            'path': And(str, os.path.exists, error='dataset path must exist on filesystem')
        },
        'preprocessing': {
            'f_sample': And(Or(int, float), ispu_valid_odr, error='invalid sensor ODR (look at the sensor docs)'),
            'full_scale_g': And(int, ispu_valid_full_scale, error='invalid sensor full scale (look at the sensor docs)'),
            Optional('power_mode', default='high_performance'):
                And(str, ispu_valid_powermode,
                    error='power_mode must be one between high_performance, low_power'),
            'win_len_samples': And(int, is_positive, error='input window length must a strictly positive integer'),
            Optional('lp_cut', default=None): Or(int, float, None, error='lp_cut must be a float between 0 and f_sample / 2 or null'),
            Optional('hp_cut', default=1): Or(int, float, error='hp_cut must be a float between 0 and f_sample / 2'),
            Optional('win_ths_g', default=0.25): Or(int, float, error='win_ths_g must be a float'),
            Optional('trig_len_samples', default=1): And(int, is_positive, error='trig_len_samples must be a strictly positive integer'),
            Optional('reset_len_samples', default=0): And(int, is_non_negative, error='reset_len_samples must be a non-negative integer'),
            Optional('discard_len_samples', default=0): And(int, is_non_negative, error='discard_len_samples must be a non-negative integer'),
            Optional('show_segmentation', default=None): Or(And(str, is_valid_segmentation_mode), None, error='show segmentation must be one between raw, filtered or null')
        },
        'training': {
            Optional('epochs', default=150): And(int, is_positive, error='epochs must be a positive integer'),
            Optional('batch_size', default=32): And(int, is_positive, error='batch_size must be a strictly positive integer'),
            Optional('train_test_split', default=0.8): And(float, is_between_0_1, error='train_test_split must a float between 0 and 1'),
            Optional('train_validation_split', default=0.1): And(float, is_between_0_1, error='train_test_split must a float between 0 and 1'),
            Optional('show_stats', default=True): Use(bool, error='show_stats must be a boolean')
        },
        'application':
        {
            Optional('prediction_threshold', default=0.6): And(float, is_between_0_1, error='prediction_threshold must a float between 0 and 1')
        }
    }
)


def segment_gestures(
        df: pd.DataFrame,
        fs: int,
        win_ths: float,
        win_len: int,
        trig_len: int,
        reset_len: int,
        discard_len: int = 0,
        show_plot: str = None,
        log_path: pathlib.Path = None
) -> List[np.ndarray]:
    """Segment dataset into windows containing one gesture each.

    Args:
            df (pd.DataFrame): Unsegmented dataframe.
            fs (int): Application sample frequency (i.e., sensor ODR)
            win_ths (float): Window threshold value.
            win_len (int): Window sample length.
            trig_len (int): Trigger event sample length.
            reset_len (int): Wait samples after each window.
            discard_len (int): Discard starting samples. Defaults to 0.

    Returns:
            Tuple(List[np.ndarray], List): List of segmented windows, list of indexes
    """
    update_len = win_len + reset_len
    i = discard_len
    win_list = []
    indexes = []
    # Start segmentation
    while (i + win_len < len(df)):
        if np.all(df.ax[i: i + trig_len] > win_ths):
            i += trig_len
            segment = df.values[i: i + win_len, :]
            if len(segment) == win_len:
                indexes.append(i)
                win_list.append(segment)
            i += update_len
        i += 1
    return win_list, indexes


def get_header_line(csv_path: str) -> int:
    """Get line number of the header

    Args:
            csv_path (str): Path to the csv file.

    Returns:
            int: line number of the header file.
    """
    header_line = 0
    with open(csv_path, 'r') as f:
        for line in f:
            if line == 'acc_x[mg],acc_y[mg],acc_z[mg]\n':
                break
            if line != '\n':
                header_line += 1

    return header_line


def fs2sens(fs: int):
    """Return the correct sensitivity for the specified full-scale setting

    Args:
            fs (int): Full scale setting.
    """
    return __SUPPORTED_FS[fs] if ispu_valid_full_scale(fs) else 0


def yaml2header(model_dir: str, user_config: dict, iir2_a: tuple, iir2_b: tuple) -> None:
    """Produce output header file with algorithm defines

    Args:
            model_dir (str): Path to the model folder.
            user_config (dict): Dictionary with the user configurations.
            iir2_a (tuple): a coefficients of the iir2 filter
            iir2_b (tuple): b coefficients of the iir2 filter
    """
    offline_confs = ['show_segmentation',
                     'discard_len_samples', 'hp_cut', 'lp_cut', 'power_mode']

    def format_c_const(n):
        return (type(n) == int and str(n)) or (type(n) == float and str(n) + 'f')

    header_file = os.path.join(model_dir, 'ispu_wand_params.h')
    shutil.copy(pathlib.Path(__file__).parent / 'license_c.txt', header_file)
    with open(header_file, 'a') as f:
        header_guard = '#ifndef ISPU_WAND_PARAMS_H\n' + \
            '#define ISPU_WAND_PARAMS_H\n\n'
        header_guard_end = '\n#endif //ISPU_WAND_PARAMS_H\n'

        defines = ''
        defines += '#define NUM_AXES 3\n'

        for k, v in sorted(user_config['preprocessing'].items(), key=lambda el: el[0]):
            if k not in offline_confs:
                defines += f'#define {k.upper()} {format_c_const(v)}\n'

        defines += f'#define NUM_LABELS {len(user_config["labels"])}\n'
        pred_ths = user_config['application']['prediction_threshold']
        defines += f'#define PRED_THS {pred_ths}f\n\n'

        filters = f'const float IIR2_B[] = {{ {iir2_b[0]}f, {iir2_b[1]}f, {iir2_b[2]} }};\n' \
            f'const float IIR2_A[] = {{ {iir2_a[0]}f, {iir2_a[1]}f, {iir2_a[2]} }};\n'

        labels = 'const char LABELS[] = { '
        labels_enum = '\nenum {\n'

        for k, v in sorted(user_config['labels'].items(), key=lambda el: el[0]):
            labels += f'\'{v.upper()}\', '
            labels_enum += f'\tLABEL_{v.upper()},\n'
        labels = labels + '\' \' };\n'
        labels_enum += '\tLABEL_NULL,\n};\n'

        f.write(header_guard)
        f.write(defines)
        f.write(filters)
        f.write(labels)
        f.write(labels_enum)
        f.write(header_guard_end)


def json_out_entry(out_name: str, out_type: str) -> str:
    return f'\t\t{{\n\t\t\t"name": "{out_name}",\n\t\t\t"type": "{out_type}"\n\t\t}}'


def yaml2json(model_dir: str, user_config: dict):
    """Produce output json file for ISPU output parsing

    Args:
            model_dir (str): Path to the model folder.
            user_config (dict): Dictionary with the user configurations.
    """
    js = '{\n\t"output":\n\t[\n'

    js += json_out_entry("acc_x [mg]", "float") + ',\n'
    js += json_out_entry("acc_y [mg]", "float") + ',\n'
    js += json_out_entry("acc_z [mg]", "float") + ',\n'
    js += json_out_entry("go", "uint8_t") + ',\n'

    for k, v in sorted(user_config['labels'].items(), key=lambda el: el[0]):
        js += json_out_entry(v.upper(), "float") + ',\n'

    js += json_out_entry("pred_char", "char") + '\n'
    js += '\t]\n}\n'
    json_file = os.path.join(model_dir, 'ispu_wand.json')
    with open(json_file, 'w') as f:
        f.write(js)


def yaml2conf(model_dir: str, user_config: str):
    """Produce output conf.txt file with sensor configurations

    Args:
            model_dir (str): Path to the model folder.
            user_config (dict): Dictionary with the user configurations.
    """
    odr = user_config['preprocessing']['f_sample']
    odr = int(odr) if odr - int(odr) == 0 else odr
    full_scale = int(user_config['preprocessing']['full_scale_g'])
    power_mode = user_config['preprocessing']['power_mode']
    conf = f'acc_odr {odr}\n' \
        f'acc_fs {full_scale}\n' \
        f'acc_mode {power_mode}\n\n' \
        f'ispu_irq_rate {odr}\n' \
        f'ispu_int1 enable\n' \
        f'ispu_sleep_int2 enable\n' \
        f'ispu_latch disable\n\n' \
        f'algo 0 enable\n' \
        f'algo_int1 0 enable\n'

    conf_file = os.path.join(model_dir, 'conf.txt')
    with open(conf_file, 'w') as f:
        f.write(conf)
