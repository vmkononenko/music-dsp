from abc import ABCMeta, abstractmethod
from accessify import protected
from chord import Chord, Song
import numpy as np
from glob import glob

class Dataset:
    #i - song, j - chord
    songlist = [[]]


    @protected
    def validate(self):
        pass

    @protected
    def load(self):
        pass

    def get_hmm_matrix_i(self):
        pass

    def get_hmm_matrix_t(self):
        pass

    def get_hmm_matrix_e(self):
        pass


class Iso_Dataset(Dataset):

    # assume we have chord_id dictionary which maps Chords into ids
    def get_hmm_matrix_i(self, chord_id, dir_path):
        chord_id = {}
        song_list = self.load(dir_path)
        i_matrix = np.array(len(chord_id), dtype=float)
        for song in song_list:
            i_matrix[chord_id.get(song.chords_list[0].label)] += 1 #для каждого аккорда получили кол-во раз, когда аккорд являлся первым в песне
        i_matrix = i_matrix/len(song_list) #оцениваем вероятность через частоту
        return i_matrix


    #returns list of Songs class instances (Song class instance contains list of Chord class instances)
    def load(self, dir_path):
        song_list = []
        file_list = glob(dir_path)
        for file in file_list:
            song_list.append(Song(self.load_chords(file)))
        return song_list

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




if __name__ == '__main__':
    Iso_Dataset().load("D:\\ChordsRecognition\\music-dsp\\tools\\DatasetExtraction\\test_chords_txt\\*.txt")

