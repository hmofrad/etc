#!/usr/bin/python3.4
# Profiling the extracted functions
# (c) Mohammad H. Mofrad, 2016
# (e) hasanzadeh@cs.pitt.edu

import pickle
import os

DEBUG = True
CSCOPE_ALL_FUNCS_BIN = 'cscope_all_funcs_fwd.dat'

file = open(CSCOPE_ALL_FUNCS_BIN, 'rb')
funcs = pickle.load(file)
file.close()

if DEBUG:
   for f in funcs:
      print(f, funcs[f])


# Calculate number of function calls for each level
max = 0
for f in funcs:
   for l in funcs[f]:
      if not isinstance(l[-1], str):
         if(l[-1] > max):
            max = l[-1]

arr = [0] * (max + 1)
for f in funcs:
   for list in funcs[f]:
      if(list[0] != None):
         print(f, ':', list[1])
         list_len = len(list)
         for l in range(2, list_len):
            arr[list[l]] = arr[list[l]] + 1
print(arr)

