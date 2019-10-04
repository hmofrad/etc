#!/usr/bin/python3.4
# (c) Mohammad H. Mofrad, 2017
# (e) mohammad.hmofradh@pitt.edu
# Support Vector Machine (SVM) implementation with cross validation using scikit

import pickle
from utils import *
import numpy as np
from sklearn import svm
from sklearn.model_selection import train_test_split
from sklearn.model_selection import cross_val_score

f = ['GDSC_sensitivity', 'GDSC_features']
v = ['labels', 'features']
d = {}
i = 0
for i in range(2):
   print('Read', f[i])

   if not os.path.isfile(f[i] + '.dat'):
      # Read in the file 
      if   i == 0:
         d[v[i]] = read_sensitivity(f[i] + '.csv')
      elif i == 1:
         d[v[i]] = read_features(f[i] + '.csv')
      # Dump the preprocessed file into a pickle file       
      file = open(f[i] + '.dat', 'wb')
      
      pickle.dump(d[v[i]], file)
      file.close()
   else:
      # Load the preprocessed pickle file into a variable
      file = open(f[i] + '.dat', 'rb')
      d[v[i]] = pickle.load(file)
      file.close()

labels = d[v[0]]
features = d[v[1]]

f = []
v = []
d = {}

'''
FILE = 'GDSC_features.dat'
if not os.path.isfile(FILE):
   FILE = 'GDSC_features.csv'
   features = read_features(FILE)
   # Dump the preprocessed features into a pickle file       
   FILE = 'GDSC_features.dat'
   file = open(FILE, 'wb')
   pickle.dump(features, file)
   file.close()
else:
   # Load the preprocessed pickle file into a variable
   print('load')
   file = open(FILE, 'rb')
   features = pickle.load(file)
   file.close()
'''

#print(labels[l])
print(len(labels[l]))
#for f in features:
#   print(f)
j = 0
d = len(list(features.values())[0])
for l in labels:
   n = len(labels[l])
   x = np.zeros((n,d))
   y = np.zeros(n)
   i = 0
   for c in labels[l]:
      y[i] = labels[l][c]
      x[i,:] = features[c][:]
      i = i + 1
   clf = svm.SVC(kernel='rbf', C = 1)
   scores = cross_val_score(clf, x, y, cv = 10)
   #logreg = linear_model.LogisticRegression(C=1e5)
   #scores = cross_val_score(logreg, x, y, cv = 10)
   print(j, ':', l)
   print(scores)
   print("Accuracy: %0.2f (+/- %0.2f)" % (scores.mean(), scores.std()))
   j = j + 1   

   file = open('out.txt', 'a')
   file.write('%s %s %s %s\n' %(l, scores.mean(), scores.std(), n))
   file.close()
   




'''
r = np.zeros(len(labels))
m = 0
k = 10
for l in labels:
   print(l)
   n = 0
   x = []
   y = []
   for c in labels[l]:
      if labels[l][c] == 'NA':
         continue
      else:
         x.append([])
         if labels[l][c] != 'FALSE':
            y.append(0.0)
            for i in range(0, len(features[c])):
               x[n].append(float(features[c][i]))
         elif labels[l][c] != 'TRUE':
            y.append(1.0)
            for i in range(0, len(features[c])):
               x[n].append(float(features[c][i]))
         n = n + 1

#   t = np.zeros(k)
#   idx = np.arange(n)
'''

