# PerfectlyParallel
  Pthread based library for having parallel I/O and processing.

#### Install
~~~
   make install
~~~

#### Clean
~~~
   make clean
~~~

#### Sequential read implementation (sequential_read.c) 
~~~
  ./sequential_read -f <file> -s <buffer size>
~~~

#### Parallel read implementation (parallel_read.c) 
~~~
  ./parallel_read -f <file> -s <buffersize> -n <numthreads>
~~~

#### TODO: Implement write interface 
