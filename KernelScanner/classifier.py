#!/usr/bin/python3.4
# Profiling the extracted functions
# (c) Mohammad H. Mofrad, 2016
# (e) hasanzadeh@cs.pitt.edu

import pickle

CSCOPE_ALL_FUNCS_BIN = 'cscope_all_funcs.dat'
file = open(CSCOPE_ALL_FUNCS_BIN, 'rb')
all_funcs = pickle.load(file)
file.close()


c = 0
j = 0
append = True
safe_funcs = {}
unsafe_funcs = {}

s = 0
u = 0
for func in all_funcs:
   for list in all_funcs[func]:
      str = list[1].split('/')
      path = str[2]
      if(list[0] == None and func != 'No'):
         if not path in safe_funcs:
            s = s + 1
            safe_funcs[path] = []
            safe_funcs[path].append(func)
         elif not func in safe_funcs[path]:
            s = s + 1
            safe_funcs[path].append(func)
      if(list[0] != None and func != 'No'):
         if not path in unsafe_funcs:
            u = u + 1
            unsafe_funcs[path] = []
            unsafe_funcs[path].append(func)
         elif not func in unsafe_funcs[path]:
            u = u + 1
            unsafe_funcs[path].append(func)

print('safe:')          
ss = 0; 
file = open('safe_funcs.txt', 'w')
for p in safe_funcs:
   print(len(safe_funcs[p]), p, ':', safe_funcs[p], file=file)
   print(p,':', len(safe_funcs[p]))
file.close

print('unsafe:')          
file = open('unsafe_funcs.txt', 'w')
for p in unsafe_funcs:
   print(len(unsafe_funcs[p]), p, ':', unsafe_funcs[p], file=file)
   print(p,':', len(unsafe_funcs[p]))
file.close


print('safe   functions', s)
print('unsafe functions', u)
print('sum    functions', s + u)

