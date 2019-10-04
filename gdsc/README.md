# Genomics of Drug Sensitivity in Cancer (GDSC)

## Requirements 
   * Linux OS
   * Python 3.4
   * Numpy: sudo pip3.4 install numpy
   * Scikit-learn: sudo pip3.4 install -U scikit-learn
   * LIBSVM: https://www.csie.ntu.edu.tw/~cjlin/libsvm/

## Dataset
   * The dataset contains 140 drugs applied on 624 cell lines and each cell line has 22712 features.
   * Contact mohammad.hmofrad@pitt.edu

## Results
   * Average results using 10 fold cross validation

| Metric   | SVM (RBF kernel) |
|----------|------------------|
| Accuracy | 0.75             |
| AUROC    | 0.57             |
