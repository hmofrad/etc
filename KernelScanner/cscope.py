#!/usr/bin/python3.4
# Extracting callee functions from Linux kernel directory using cscope library 
# (c) Mohammad H. Mofrad, 2017
# (e) hasanzadeh@cs.pitt.edu

import os
import sys
import re
import pickle
import time
import copy
from collections import namedtuple
from joblib import Parallel, delayed
import multiprocessing


DEBUG = False
FUNC_ = namedtuple('FUNC_', ['path', 'visited'])

CALLEE_FUNC = 'write_seqlock'
CSCOPE_FILES = 'cscope.files'
NCSCOPE_OUT = 'ncscope.out'
CSCOPE_OUT = 'cscope.out'
CSCOPE_OUTPUT_FILE = 'cscope_output.txt'
CSCOPE_OUTPUT_FILE_1 = 'cscope_output_1.txt'
CSCOPE_ALL_FUNCS = 'cscope_all_funcs.txt'
CALLEE_OUTPUT_FILE = 'callee_output.txt'
CALLEE_OUTPUT_FILE_1 = 'callee_output_1.txt'
CSCOPE_ALL_FUNCS_BIN = 'cscope_all_funcs.dat'
CSCOPE_ALL_FUNCS_CLEAN_BIN = 'cscope_all_funcs_clean.dat'
HOME_DIR = '/home/mofrad'
TMP = 'tmp/'

FWD = 'FORWARD'
BWD = 'BACKWARD'
DIRECTION = FWD
funcs  = {}
f_list = []

#funcs_list = {}
r = 1 # Run index
k = 1

NUM_CORES = multiprocessing.cpu_count()

# Clean all generated files
def sweep():   
   print('Cleaning previous build');
   if os.path.isfile(CSCOPE_FILES):
      os.remove(CSCOPE_FILES)
   if os.path.isfile(CSCOPE_OUT):
      os.remove(CSCOPE_OUT)
   if os.path.isfile(NCSCOPE_OUT):
      os.remove(NCSCOPE_OUT)
   if os.path.isfile(CSCOPE_OUTPUT_FILE):
      os.remove(CSCOPE_OUTPUT_FILE)
   if os.path.isfile(CSCOPE_ALL_FUNCS):
      os.remove(CSCOPE_ALL_FUNCS)
   if os.path.isfile(CSCOPE_ALL_FUNCS_BIN):
      os.remove(CSCOPE_ALL_FUNCS_BIN)
   if os.path.isfile(CSCOPE_ALL_FUNCS_CLEAN_BIN):
      os.remove(CSCOPE_ALL_FUNCS_CLEAN_BIN)
   if os.path.isfile(CALLEE_OUTPUT_FILE):
      os.remove(CALLEE_OUTPUT_FILE)

def resume(r):
   for f in funcs:
      for l in funcs[f]:
         if isinstance(l[-1], int) and l[-1] > r:
            r = l[-1]
   return(r)

# Call cscope library
# Direction = [Forward scan | Backward scan] 
def cscope(direction, function, file):
   if direction == BWD:
      cmd = 'cscope -L -2 \'%s\' > ' %function + file
   elif  direction == FWD:
      cmd = 'cscope -L -3 \'%s\' > ' %function + file
   else:
      printf('Error on direction')
      exit(0)
   if DEBUG:
      print(cmd)
   os.system(cmd)


def scan(FILE, func_callee):
   file = open(FILE, 'r')
   lines = tuple(file)
   file.close()
   for line in lines:
      # The helper regular expression that captures almost anything
      # that is follows the cscope output convention 
      helper_regex = re.search(r'^(\/.+\/[a-zA-Z0-9_#\+\-\.\,]+)\s+([a-zA-Z0-9_#\+\-\.]+)\s+[0-9]+.*$', line)
      if not helper_regex:
         print(line, end = '', flush = True)
         print('ERROR: Devise a new regular expression')
         exit(0)
 
      path = helper_regex.group(1)
      path = re.sub(r'^%s'%HOME_DIR, '', path)
      func_caller = helper_regex.group(2)
      if DEBUG:
         print(line, end = '', flush = True)
         print('File path:', path)
         print('Caller function:', func_caller)
         print('Callee function:', func_callee)

      # Keep bookkeeping information
      # for new founded functions
      # Update the function database
      f = func_caller

      append = True
      if f in funcs:
         for i in range(0, len(funcs[f])):
            if funcs[f][i][0] == None and funcs[f][i][1] == path:
               funcs[f][i][0] = func_callee
               if(isinstance(funcs[f][i][-1], str)):
                  funcs[f][i].append(r)
               append = False
               break
            elif funcs[f][i][0] == func_callee and funcs[f][i][1] == path:
               if(isinstance(funcs[f][i][-1], str)):
                  funcs[f][i].append(r)
               append = False
               break
         if append:
            funcs[f].append([func_callee, path])
            if(funcs[f][-1][-1]):
               funcs[f][-1].append(r)
         append = True  
      # Add the new function to the function database
      else:
         funcs[f] = []
         funcs[f].append([func_callee, path])
         funcs[f][-1].append(r)
   
argc = len(sys.argv)
argv =  sys.argv

if DEBUG:
   print('Number of arguments:', len(sys.argv), 'arguments.')
   print('Argument list:', str(sys.argv))

if (argc == 2) and (argv[1] == 'sweep'):
   sweep()
   exit(0)
elif ((argc == 2) and (argv[1] != 'sweep') and (argv[1] != 'resume') and (argv[1] != 'reverse')) or (argc > 2):
   print('USAGE: python3.4 cscope.py [sweep]')
   exit(0)

start_time = time.time()

# Delete the last garbage lines just for the 1st time 
if not os.path.isfile(CSCOPE_FILES) or not os.path.isfile(CSCOPE_OUT):
   print('creating', CSCOPE_FILES);
   cmd = './cscope.sh'
   if DEBUG:
      print(cmd)
   os.system(cmd)
   cmd = 'sed -i \'/^\.\.\/.*$/d\' cscope.files'
   if DEBUG:
      print(cmd)
   os.system(cmd)
else:
   print(CSCOPE_FILES, 'is already existed');

# if cscope database is generated,
# parse the database for function names and their path
# and populate the func list 
if not os.path.isfile(CSCOPE_ALL_FUNCS):
   print('creating', CSCOPE_ALL_FUNCS);
   cscope(BWD,'.*', CSCOPE_ALL_FUNCS)
   if os.path.isfile(CSCOPE_ALL_FUNCS): 
      file = open(CSCOPE_ALL_FUNCS, 'r')
      lines = tuple(file)
      file.close()
      for line in lines:    
         path = str.split(line)[0]
         path = re.sub(r'^%s'%HOME_DIR, '', path)
         func_caller = str.split(line)[1]
         func_callee = None
         f = func_caller
         if f in funcs:
            if all(ff[1] != path for ff in funcs[f]):
               funcs[f].append([func_callee, path])
         else:
            funcs[f] = []
            funcs[f].append([func_callee, path])
         
      # Dump the database into a pickle file       
      file = open(CSCOPE_ALL_FUNCS_BIN, 'wb')
      pickle.dump(funcs, file)
      file.close()

      # For clean resume only
      file = open(CSCOPE_ALL_FUNCS_CLEAN_BIN, 'wb')
      pickle.dump(funcs, file)
      file.close()
       
   else:
      # if cscope database is not generated and I'm here
      # throw an error
      print('Error opening ', CSCOPE_ALL_FUNCS)
      exit(0)        
      
# If the dumped cscope database exists
# Populate the cscope database data structure
if os.path.isfile(CSCOPE_ALL_FUNCS_BIN): 
   # Resume from the last level of function calls
   if (argc == 2) and (argv[1] == 'resume'):
      print('cscope.py: resume from level: ',end="", flush=True)
      file = open(CSCOPE_ALL_FUNCS_BIN, 'rb')
      funcs = pickle.load(file)
      file.close()
      r = resume(r)
      print(r)
   if (argc == 2) and (argv[1] == 'reverse'):
      file = open(CSCOPE_ALL_FUNCS_BIN, 'rb')
      funcs = pickle.load(file)
      file.close()
      #r = resume(r)
      #print(r)
      r = -1
      k = -1
      DIRECTION = BWD
      cscope(DIRECTION, CALLEE_FUNC, CSCOPE_OUTPUT_FILE)
      scan(CSCOPE_OUTPUT_FILE, CALLEE_FUNC)
   else:
      # Start from the beginning
      print('CSCOPE + SCAN + START')
      file = open(CSCOPE_ALL_FUNCS_CLEAN_BIN, 'rb')
      funcs = pickle.load(file)
      file.close()
      cscope(DIRECTION, CALLEE_FUNC, CSCOPE_OUTPUT_FILE)
      scan(CSCOPE_OUTPUT_FILE, CALLEE_FUNC)   
else:
   print('Error opening ', CSCOPE_ALL_FUNCS_BIN)
   exit(0)  

n = NUM_CORES
f_num = []
f_list_tmp = []

while True:
   r = r + k
   f_list_tmp[:] = []
   for f in funcs:
#      print(f, ':', funcs[f])
      if any(fl[0] == None  for fl in funcs[f]):
         for l in funcs[f]:
            if(l[-1] == r - k):
               f_list_tmp.append(f)
               break
   if len(f_list_tmp) == 0:
      break

   f_num.append(len(f_list_tmp))
      
   print('Round #',r, ': launching scanner')
   print('# functions to scan in the this round: ', len(f_list_tmp))
   for i in range(0, len(f_list_tmp), NUM_CORES):
      print('Function left for scan: ', len(f_list_tmp) - i)
      functions = f_list_tmp[i:i+NUM_CORES]
      if DEBUG:
         #pass   
         print('Functions to scan', functions)
      # Parallel execution of cscope library
      # using python multiprocessing package 
      Parallel(n_jobs=NUM_CORES)(delayed(cscope)(DIRECTION, f, TMP+f) for f in functions)
      for f in functions:
         scan(TMP+f, f)
   print('Total number of functions: ', len(f_list))
   
   
   cmd = 'rm -rf %s' %(TMP)
   if DEBUG:
      print(cmd)
   os.system(cmd)
   cmd = 'mkdir %s' %(TMP)
   os.system(cmd)

   file = open(CSCOPE_ALL_FUNCS_BIN, 'wb')
   pickle.dump(funcs, file)
   file.close()


# Dump the database into a pickle file
# Uncomment this to save results       
file = open(CSCOPE_ALL_FUNCS_BIN, 'wb')
pickle.dump(funcs, file)
file.close()

file = open(CALLEE_OUTPUT_FILE, 'w')
for f in funcs:
   print(f, funcs[f], file=file)
file.close()


elapsed_time = time.time() - start_time
print('Elapsed time:', elapsed_time, 'seconds')
