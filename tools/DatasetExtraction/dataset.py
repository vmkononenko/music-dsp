from abc import ABCMeta, abstractmethod
from accessify import private, protected
import chord

class Dataset:
    #i - song, j - chord
    songlist = [[]]

    @protected
    def validate(self):
        for song in self.songlist:
            for ch in song:
                if not isinstance(ch, chord):
                    raise Exception("Incorrect chord type")
                    #will also chech cq_max


    #returns songlist with Chord class instances
    @protected
    @abstractmethod
    def load(self):
        pass

    @private
    def get_hmm_matrix_i(self):
        pass

    @private
    def get_hmm_matrix_t(self):
        pass

    @private
    def get_hmm_matrix_e(self):
        pass



