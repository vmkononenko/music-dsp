#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse

"""
   Train chord templates HMM
"""

# usage
# ./tct_hmm.py -d /path/to/dataset1 -d /path/to/dataset2 --cqmax hdim7 -m I
#

def get_init_prob_matrix():
    ret = "test"
    return ret

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-d', '--dataset',
                        type=str, required=True, nargs='+',
                        help='Path to datasets separates with whitespace')
    parser.add_argument('-q', '--cqmax',
                        type=str, required=True,
                        choices=['maj', 'min'],
                        help='Max chord quality supported by the library, in Harte\'s shorthand notation')
    parser.add_argument('-m', '--matrix',
                        type=str, required=True,
                        choices=['I', 'T', 'E'],
                        help='Type of HMM probability matrix to be trained: I - initial,\
                              T - transitional or E - emission')
    args = parser.parse_args()

    out = ""
    if args.matrix == 'I':
        out = get_init_prob_matrix()

    print out

if __name__ == "__main__":
    main()
