##############################################################################
# Copyright(c) 2024 STMicroelectronics.
# All rights reserved.
#
# This software is licensed under terms that can be found in the LICENSE file
# in the root directory of this software component.
# If no LICENSE file comes with this software, it is provided AS-IS.
##############################################################################

__all__ = ["segment_gestures"]

from typing import List, Tuple

import numpy as np
import pandas as pd


def segment_gestures(
    df: pd.DataFrame,
    win_ths: float,
    win_len: int,
    trig_len: int,
    reset_len: int,
    discard_len: int = 0
) -> Tuple[List[np.ndarray], List[int]]:
    """Segment dataset into windows containing one gesture each.

    Args:
        df (pd.DataFrame): Unsegmented dataframe.
        win_ths (float): Window threshold value.
        win_len (int): Window sample length.
        trig_len (int): Trigger event sample length.
        reset_len (int): Wait samples after each window.
        discard_len (int): Discard starting samples. Defaults to 0.

    Returns:
        Tuple[List[np.ndarray], List[int]]: List of segmented windows, list of
            segmented windows start indexes.
    """
    update_len = win_len + reset_len
    i = discard_len
    win_list = []
    indexes = []
    # Start segmentation
    while (i + win_len < len(df)):
        tmp = df.iloc[i:i + trig_len]
        if np.all(tmp["ax"] > win_ths):
            i += trig_len
            win = df.iloc[i:i + win_len]
            if len(win) == win_len:
                indexes.append(i)
                win_list.append(win.to_numpy())
            i += update_len
        i += 1
    return win_list, indexes
