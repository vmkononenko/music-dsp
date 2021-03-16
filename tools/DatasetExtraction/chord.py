"""
Simple chord implementation

Notes
-----
This implementation tries to follow the references and their implementation
(e.g., https://github.com/jayg996/BTC-ISMIR19)
"""


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
