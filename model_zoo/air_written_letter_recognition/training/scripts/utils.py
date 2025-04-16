##############################################################################
# Copyright(c) 2024 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
##############################################################################

__all__ = [
    "get_sensitivity",
    "get_csv_header",
    "yaml_to_header",
    "yaml_to_meta",
    "yaml_to_conf",
    "validate_user_config_yaml"
]

from typing import Dict, Set
import os
import shutil
import pathlib

from schema import Schema, Use, And, Or, Optional

# Output filenames for generated files
_ISPU_WAND_PARAMS_H_FILENAME = "ispu_wand_params.h"
_ISPU_WAND_PARAMS_H_GUARD = "ISPU_WAND_PARAMS_H"
_LICENSE_C_TXT_FILENAME = "license_c.txt"
_META_TXT_FILENAME = "meta.txt"
_CONF_TXT_FILENAME = "conf.txt"

# Sensor available configurations
_SUPPORTED_POWER_MODES = { "low_power", "high_performance" }
_SUPPORTED_ODR = { 12.5, 26, 52, 104, 208, 416, 833, 1667, 3333, 6667 }
_SUPPORTED_FS: Dict[int, float] = { 2: 0.000061, 4: 0.000122, 8: 0.000244, 16: 0.000488 }


def get_sensitivity(fs: int) -> float:
    """Return the correct sensitivity for the specified full-scale setting.

    Args:
        fs (float): Full scale setting.

    Returns:
        float: Sensitivity value.
    """
    return _SUPPORTED_FS.get(fs, 0.0)


def get_csv_header(csv_path: str) -> int:
    """Get line number of the header.

    Args:
        csv_path (str): Path to the csv file.

    Returns:
        int: line number of the header file.
    """
    header_line = 0
    with open(csv_path, "r", encoding="utf-8") as f:
        for line in f:
            if line == "acc_x[mg],acc_y[mg],acc_z[mg]\n":
                break
            if line != "\n":
                header_line += 1
    return header_line


def yaml_to_header(
    model_dir: str, user_config: dict, iir2_a: tuple, iir2_b: tuple
) -> None:
    """Produce output header file with algorithm defines.

    Args:
        model_dir (str): Path to the model folder.
        user_config (dict): Dictionary with the user configurations.
        iir2_a (tuple): a coefficients of the iir2 filter.
        iir2_b (tuple): b coefficients of the iir2 filter.
    """
    offline_confs = [
        "power_mode",
        "hp_cut",
        "lp_cut",
        "discard_len_samples",
        "show_segmentation"
    ]

    def format_c_const(n):
        return (
            (isinstance(n, int) and str(n)) or
            (isinstance(n, float) and str(n) + "f")
        )

    header_file = os.path.join(model_dir, _ISPU_WAND_PARAMS_H_FILENAME)
    shutil.copy(pathlib.Path(__file__).parent / _LICENSE_C_TXT_FILENAME, header_file)
    with open(header_file, "a", encoding="utf-8") as f:
        defines = ""
        for key, val in {
            **user_config["preprocessing"], **user_config["application"]
        }.items():
            if key not in offline_confs:
                defines += f"#define {key.upper()} {format_c_const(val)}\n"
        defines += "#define NUM_AXES 3\n"
        defines += f"#define NUM_LABELS {len(user_config['labels'])}\n"
        defines += (
            "#define IIR2_B { " +
            ", ".join([f"{x:f}f" for x in iir2_b]) + " }\n" +
            "#define IIR2_A { " +
            ", ".join([f"{x:f}f" for x in iir2_a]) + " }\n"
        )

        labels_enum = "\nenum {\n"
        labels = "const char LABELS[] = { "
        for label, _ in sorted(user_config["labels"].items(), key=lambda el: el[1]):
            labels_enum += f"\tLABEL_{label.upper()},\n"
            labels += f"'{label.upper()}', "
        labels_enum += "\tLABEL_NULL,\n};\n"
        labels = labels + "' ' };\n"

        f.write(f"#ifndef {_ISPU_WAND_PARAMS_H_GUARD}\n")
        f.write(f"#define {_ISPU_WAND_PARAMS_H_GUARD}\n\n")
        f.write(defines)
        f.write(labels_enum)
        f.write(labels)
        f.write(f"\n#endif //{_ISPU_WAND_PARAMS_H_GUARD}\n")


def yaml_to_meta(model_dir: str, user_config: dict) -> None:
    """Produce output meta.txt file with metadata.

    Args:
        model_dir (str): Path to the model folder.
        user_config (dict): Dictionary with the user configurations.
    """
    meta = f'description "ISPU wand ({user_config["general"]["name"]})"\n\n'

    meta += 'output "acc_x [mg]" float\n'
    meta += 'output "acc_y [mg]" float\n'
    meta += 'output "acc_z [mg]" float\n'
    meta += 'output "go" uint8_t\n'

    for label, _ in sorted(user_config["labels"].items(), key=lambda el: el[1]):
        meta += f'output "{label.upper()}" float\n'

    meta += 'output "pred_char" char\n\n'

    meta_file = os.path.join(model_dir, _META_TXT_FILENAME)
    with open(meta_file, "w", encoding="utf-8") as f:
        f.write(meta)


def yaml_to_conf(model_dir: str, user_config: dict) -> None:
    """Produce output conf.txt file with sensor configurations.

    Args:
        model_dir (str): Path to the model folder.
        user_config (dict): Dictionary with the user configurations.
    """
    odr = user_config["preprocessing"]["f_sample"]
    odr = int(odr) if odr - int(odr) == 0 else odr
    full_scale = int(user_config["preprocessing"]["full_scale_g"])
    power_mode = user_config["preprocessing"]["power_mode"]
    conf = (
        f"acc_odr {odr}\n"
        f"acc_fs {full_scale}\n"
        f"acc_mode {power_mode}\n\n"
        f"ispu_irq_rate {odr}\n"
        f"ispu_int1 enable\n"
        f"ispu_sleep_int2 enable\n"
        f"ispu_latch disable\n\n"
        f"algo 0 enable\n"
        f"algo_int1 0 enable\n\n"
    )

    conf_file = os.path.join(model_dir, _CONF_TXT_FILENAME)
    with open(conf_file, "w", encoding="utf-8") as f:
        f.write(conf)


def validate_user_config_yaml(config: Dict) -> Dict:
    """Validate user config YAML file.

    Args:
        config (Dict): User config YAML object.

    Returns:
        Dict: Validated YAML object.
    """
    return _USER_CONFIG_SCHEMA.validate(config)


def _is_positive(n: int) -> bool:
    """Check if an integer is greater than zero.

    Args:
        n (int): Integer number.

    Returns:
        bool: Integer is positive.
    """
    return n > 0


def _is_non_negative(n: int) -> bool:
    """Check if an integer is greater or equal to zero.

    Args:
        n (int): Integer number.

    Returns:
        bool: Integer non negative.
    """
    return n >= 0


def _is_between_0_1(x: float) -> bool:
    """Check if a float is between 0 and 1 (including 0 and 1)

    Args:
        x (float): Float number.

    Returns:
        bool: Float is between 0 and 1.
    """
    return (x >= 0) and (x <= 1)


def _is_valid_segmentation_mode(s: str) -> bool:
    """Check if segmentation mode is 'raw' or 'filtered'.

    Args:
        s (str): Segmentation mode.

    Returns:
        bool: Segmentation mode is valid.
    """
    return s is None or s in {"raw", "filtered"}


def _is_valid_odr(odr: float) -> bool:
    """Check if the input Output Data Rate is supported by the ispu.

    Args:
        odr (float): Output Data Rate setting.

    Returns:
        bool: ODR is valid.
    """
    return odr in _SUPPORTED_ODR


def _is_valid_fs(fs: int) -> bool:
    """Check if the input full-scale is supported by the ispu.

    Args:
        fs (int): Full scale setting.

    Returns:
        bool: Full-scale is valid.
    """
    return fs in _SUPPORTED_FS


def _is_valid_powermode(power_mode: str) -> bool:
    """Check if the input power mode is valid for the ispu.

    Args:
        power_mode (str): Power mode setting.

    Returns:
        bool: Power mode is valid.
    """

    return power_mode in _SUPPORTED_POWER_MODES


_USER_CONFIG_SCHEMA = Schema({
    "general": {
        "name": Use(
            str,
            error="name of the use case must be a string"
        )
    },
    "dataset": {
        "name": Use(
            str,
            error="name of the dataset must be a string"
        ),
        "path": And(
            str,
            os.path.exists,
            error="dataset path must exist on filesystem"
        )
    },
    "preprocessing": {
        "f_sample": And(
            Or(int, float),
            _is_valid_odr,
            error="invalid sensor ODR (look at the sensor docs)"
        ),
        "full_scale_g": And(
            int,
            _is_valid_fs,
            error="invalid sensor full scale (look at the sensor docs)"
        ),
        Optional("power_mode", default="high_performance"): And(
            str,
            _is_valid_powermode,
            error="power_mode must be either high_performance or low_power"
        ),
        "win_len_samples": And(
            int,
            _is_positive,
            error="input window length must a strictly positive integer"
        ),
        Optional("lp_cut", default=None): Or(
            int,
            float,
            None,
            error="lp_cut must be a float between 0 and f_sample / 2"
        ),
        Optional("hp_cut", default=1): Or(
            int,
            float,
            error="hp_cut must be a float between 0 and f_sample / 2"
        ),
        Optional("win_ths_g", default=0.25): Or(
            int,
            float,
            error="win_ths_g must be a float",
        ),
        Optional("trig_len_samples", default=1): And(
            int,
            _is_positive,
            error="trig_len_samples must be a strictly positive integer"
        ),
        Optional("reset_len_samples", default=0): And(
            int,
            _is_non_negative,
            error="reset_len_samples must be a non-negative integer"
        ),
        Optional("discard_len_samples", default=0): And(
            int,
            _is_non_negative,
            error="discard_len_samples must be a non-negative integer"
        ),
        Optional("show_segmentation", default=None): Or(
            And(str, _is_valid_segmentation_mode),
            None,
            error="show segmentation must be either raw or filtered"
        )
    },
    "training": {
        Optional("epochs", default=150): And(
            int,
            _is_positive,
            error="epochs must be a positive integer"
        ),
        Optional("batch_size", default=32): And(
            int,
            _is_positive,
            error="batch_size must be a strictly positive integer"
        ),
        Optional("train_test_split", default=0.8): And(
            float,
            _is_between_0_1,
            error="train_test_split must a float between 0 and 1"
        ),
        Optional("train_validation_split", default=0.1): And(
            float,
            _is_between_0_1,
            error="train_test_split must a float between 0 and 1"
        ),
        Optional("show_stats", default=True): Use(
            bool,
            error="show_stats must be a boolean"
        )
    },
    "application": {
        Optional("pred_ths", default=0.6): And(
            float,
            _is_between_0_1,
            error="pred_ths must a float between 0 and 1"
        )
    }
})
