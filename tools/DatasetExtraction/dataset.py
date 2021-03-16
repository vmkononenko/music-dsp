from abc import ABCMeta, abstractmethod
from accessify import protected
from chord import Chord, Song
import numpy as np
from glob import glob

class Dataset:
    #i - song, j - chord
    song_list = []
    chord_id = {}
    dir_path = ""
    window_size = 100.0

    def __init__(self, dir_path, chord_id, window_size):
        self.dir_path = dir_path
        self.chord_id = chord_id
        self.window_size = window_size
        self.song_list = self.load()

    @protected
    def validate(self):
        pass

    #@protected
    def load(self):
        file_list = glob(self.dir_path)
        for file in file_list:
            self.song_list.append(Song(self.load_chords(file)))
        return self.song_list

    @protected
    def load_chords(self, filename):
        pass

    #returns vector with probabilities (dtype np array)
    def get_hmm_matrix_i(self):
        self.song_list = self.load(self.dir_path)
        i_matrix = np.array(len(self.chord_id), dtype=float)
        for song in self.song_list:
            i_matrix[self.chord_id.get(song.chords_list[
                                      0].label)] += 1
        i_matrix = i_matrix / len(self.song_list)
        return i_matrix

    # returns matrix with probabilities (dtype np array)
    def get_hmm_matrix_t(self):
        tran_count = 0.0
        remainder = 0.0
        drtn = 0.0
        w_cnt = 0.0
        t_matrix = np.array((len(self.chord_id), len(self.chord_id)), dtype=float)

        for song_i in self.song_list:
            t_matrix[self.chord_id.get(song_i[-1].label)][  # analyze last elem
                self.chord_id.get(song_i[-1].label)] += (song_i[-1].end_time - song_i[-1].start_time) / self.window_size
            itr = song_i[:-1]  # remove last element (because use elem+1)

            for j in range(itr):
                curr_chord = song_i.chords_list[j]
                drtn = curr_chord.end_time - curr_chord.start_time
                w_cnt = int(drtn / self.window_size)

                if (remainder > self.window_size / 2):
                    w_cnt = + 1

                remainder = drtn % self.window_size

                tran_count += w_cnt

                t_matrix[self.chord_id.get(curr_chord.label)][    #self trasition
                    self.chord_id.get(curr_chord.label)] += w_cnt

                t_matrix[self.chord_id.get(curr_chord.label.label)][    #chord changing transition
                    self.chord_id.get(song_i.chords_list[j + 1].label)] += 1
        t_matrix = t_matrix / tran_count
        return  t_matrix

    def get_hmm_matrix_e(self):
        pass


class Iso_Dataset(Dataset):

    #Load chords from a text file, returns list of Chord instances
    def load_chords(self, filename):
        chords_list = []
        with open(filename, 'r') as f:
            for line in f:
                if line:
                    splits = line.split()
                    if len(splits) == 3:
                        s = splits[0]
                        e = splits[1]
                        l = self.label_error_modify(splits[2])
                        chords_list.append(Chord(float(s), float(e), l))
        return chords_list

    #removes label missprints
    def label_error_modify(self, label):
        if label == 'Emin/4':
            label = 'E:min/4'
        elif label == 'A7/3':
            label = 'A:7/3'
        elif label == 'Bb7/3':
            label = 'Bb:7/3'
        elif label == 'Bb7/5':
            label = 'Bb:7/5'
        elif label.find(':') == -1:
            if label.find('min') != -1:
                label = label[:label.find('min')] + ':' + label[label.find('min'):]
        return label





