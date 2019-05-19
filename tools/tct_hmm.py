#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2019 Volodymyr Kononenko
#
# This file is part of Music-DSP.
#
# Music-DSP is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Music-DSP is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with Music-DSP. If not, see <https://www.gnu.org/licenses/>.

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
