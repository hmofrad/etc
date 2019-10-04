#!/usr/bin/python3.4
# (c) Mohammad H. Mofrad, 2017
# (e) mohammad.hmofradh@pitt.edu
# Utility functions

import numpy as np
import os

VERBOSE = True

# Open the file and read its content
def file_open(FILE):
   if os.path.isfile(FILE):
      file = open(FILE, 'r')
      lines = tuple(file)
      file.close()
   else:
      print(FILE, 'does not exist')
      exit(0)

   return(lines)

# Read gdsc labels 141 x 625 ==> 140 x 624 [drug x cell line]
def read_sensitivity(FILE):
   lines = file_open(FILE)
 
   raw  = []
   labels = {}
   for line in lines:
      raw.append(line.rstrip().split(",")) # Convert tuple to list
   height = len(raw)
   width = len(raw[0])
   if VERBOSE:
      print(height, 'x', width)

   for i in range(1, height):
      labels[raw[i][0]] = {}
      for j in range(1, width):
         if raw[i][j] != '':
            labels[raw[i][0]][raw[0][j]] = float(raw[i][j])

   return(labels)

# Read gdsc features 625 x 22712 ==> 624x22712 [cell line x feature]
def read_features(FILE):
   lines = file_open(FILE)

   raw  = []
   features = {}
   for line in lines:
      raw.append(line.rstrip().split(",")) # Convert tuple to list
   height = len(raw)
   width  = len(raw[0])
   if VERBOSE:
      print(height, 'x', width)

   for i in range(1, width):
      raw[0][i] = raw[0][i].strip('"')
   for j in range(1, height):
      raw[j][0] = raw[j][0].strip('"')

   for i in range(1, height):
      for j in range(1,width):
         raw[i][j] = float(raw[i][j])
      features[raw[i][0]] = raw[i][1:]
   return(features)
