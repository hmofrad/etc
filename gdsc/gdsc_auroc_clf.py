#!/usr/bin/python3.4
# (c) Mohammad H. Mofrad, 2017
# (e) mohammad.hmofrad@pitt.edu
# Support Vector Machine (SVM) implementation with cross validation 
# and Area Under Receiver Operating Characteristic (AUROC) using scikit

import pickle
from utils import *
import numpy as np
from sklearn import svm
from sklearn.model_selection import train_test_split
from sklearn.model_selection import cross_val_score
from sklearn import linear_model

from sklearn.preprocessing import label_binarize
from sklearn.metrics import roc_curve, auc
from sklearn.model_selection import train_test_split
from sklearn.model_selection import StratifiedKFold

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

x = []
y = []

j = 0
d = len(list(features.values())[0])
k = 10
stat = np.zeros((2,k))
 
for l in labels:
   n = len(labels[l])
   x = np.zeros((n,d))
   y = np.zeros(n)
   i = 0
   print('Drug:', l)
   for c in labels[l]:
      y[i] = labels[l][c]
      x[i,:] = features[c][:]
      i = i + 1
   j = 0
   random_state = np.random.RandomState(0)
   classifier = svm.SVC(kernel='linear', probability=True, random_state=random_state)
   cv = StratifiedKFold(n_splits = k)
   for (train, test) in cv.split(x, y):
      
#      y_probas = classifier.fit(x[train, :], y[train]).predict_proba(x[test,:])
      y_scores  = classifier.fit(x[train, :], y[train]).decision_function(x[test,:])
      y_predict = classifier.predict(x[test,:])

      fpr, tpr, _ = roc_curve(y[test], y_predict)
      roc_auc = auc(fpr, tpr)
      print('Fold%d' %(j), 'AUROC:    %0.2f' %(roc_auc))
      stat[0, j] = roc_auc

      count = 0
      for i in range(len(y_predict)):
         if(y_predict[i] == y[test][i]):
            count = count + 1
      print('Fold%d' %(j), 'Accuracy: %0.2f' %(count/len(y_predict)))
      stat[1, j] = count/len(y_predict)

      j = j + 1
   print('AUROC,  Accuracy')
   mean = np.mean(stat, axis = 1)
   std  = np.std(stat, axis = 1)
   print(mean, std)
   print("AUROC: %0.2f (+/- %0.2f)" % (mean[0], std[0]))
   print("Accuracy: %0.2f (+/- %0.2f)" % (mean[1], std[1]))

   file = open('out.txt', 'a')
   file.write('%s %s %s %s %s\n' %(l, mean[0], std[0], mean[1], std[1]))
   file.close()

