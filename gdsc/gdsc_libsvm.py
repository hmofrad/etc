#!/usr/bin/python3.4
# (c) Mohammad H. Mofrad, 2017 
# (e) mohammad.hmofradh@pitt.edu
# Support Vector Machine (SVM) implementation using libsvm

import numpy as np
import sys
import os
sys.path.append("/usr/src/libsvm-3.22/python/")
from svmutil import *

# Debug level
VERBOSE = False

# Read gdsc labels 141 x 625 ==> 140 x 624 [drug x cell line]
FILE = 'GDSC_sensitivity.csv'
if os.path.isfile(FILE):
   file = open(FILE, 'r')
   lines = tuple(file)
   file.close()
else:
   print(FILE, 'does not exist')
   exit(0)

raw  = []
labels = {}
for line in lines:
   raw.append(line.rstrip().split(",")) # Convert tuple to list
height = len(raw)
width = len(raw[0])
if VERBOSE:
   print(height, width)

for i in range(1, height):
   labels[raw[i][0]] = {}
   for j in range(1, width):
      labels[raw[i][0]][raw[0][j]] = raw[i][j]

# Read gdsc features 625 x 22713 ==> 624x22712 [cell line x feature]
FILE = 'GDSC_features.csv'
if os.path.isfile(FILE):
   file = open(FILE, 'r')
   lines = tuple(file)
   file.close()
else:
   print(FILE, 'does not exist')
   exit(0)
raw  = []
features = {}

for line in lines:
   raw.append(line.rstrip().split(",")) # Convert tuple to list
height = len(raw)
width = len(raw[1])
if VERBOSE:
   print(height, width)

for i in range(0, width-1):
   raw[0][i] = raw[0][i].strip('"')
for j in range(1, height):
   raw[j][0] = raw[j][0].strip('"')

for i in range(1, height):
   features[raw[i][0]] = raw[i][1:]

m = len(labels)
r = np.zeros(m)
o = 0
k = 10
for l in labels:
   n = 0
   y = []
   x = []
   for c in labels[l]:
      if labels[l][c] == 'NA':
         continue
      else:
         x.append([])
         if labels[l][c] != 'FALSE':
            y.append(0.0)
            for i in range(1, width):
               x[n].append(float(features[c][i-1]))
         elif labels[l][c] != 'TRUE':
            y.append(1.0)
            for i in range(1, width):
               x[n].append(float(features[c][i-1]))
         n = n + 1

   t = np.zeros(k)
   idx = np.arange(n)
   for i in range(k):
      idx_test = np.arange(start = i, stop = n, step = k)
      idx_train = np.setdiff1d(idx, idx_test)
      y_ = []
      y_ = [y[j] for j in idx_train]
      x_ = []
      x_ = [x[j][:] for j in idx_train]

      s = svm_train(y_, x_, '-t 0')
      y_ = []
      y_ = [y[j] for j in idx_test]
      x_ = []
      x_ = [x[j][:] for j in idx_test]
      p_label, p_acc, p_val = svm_predict(y_, x_, s)
      t[i] = p_acc[0]
   r[o] = np.mean(t[i])
   file = open('out.txt', 'a')
   file.write('%s %s\n' %(l, r[o]))
   file.close()
   print(l, r[o])
   o = o + 1

#for d in labels:
#   file = open('data/' + d, 'w')
#   for c in labels[d]:
#      if labels[d][c] == 'NA':
#         continue
#      else:
#         if labels[d][c] != 'FALSE':
#            file.write('0 ')
#         elif labels[d][c] != 'TRUE':
#            file.write('1 ')
#         for i in range(1, width):
#            if i+1 == width:
#               file.write('%s:%s' %(i, features[c][i-1]))
#            else:
#               file.write('%s:%s ' %(i, features[c][i-1]))
#         file.write('\n')
#   file.close()

