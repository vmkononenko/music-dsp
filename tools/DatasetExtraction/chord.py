"""
Simple chord implementation

Notes
-----
This implementation tries to follow the references and their implementation
(e.g., https://github.com/jayg996/BTC-ISMIR19)
"""

import numpy as np
import pandas as pd

CHORD_DTYPE = [('root', np.int),
               ('bass', np.int),
               ('intervals', np.int, (12,)),
               ('is_major', np.bool)]

CHORD_ANN_DTYPE = [('start', np.float),
                   ('end', np.float),
                   ('chord', CHORD_DTYPE)]

NO_CHORD = (-1, -1, np.zeros(12, dtype=np.int), False)
UNKNOWN_CHORD = (-1, -1, np.ones(12, dtype=np.int) * -1, False)


class Chord:
    def __init__(self, start_time, end_time, label):
        self.start_time = start_time
        self.end_time = end_time
        self.label = label

    def __repr__(self):
        return "({}, {}, {})".format(self.start_time, self.end_time, self.label)


class Song:
    def __init__(self, chords_list):
        self.chords_list = chords_list
        try:
            if chords_list[0].label == 'N':
                chords_list.pop(0)
        except:
            print("Error occured")
