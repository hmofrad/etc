# Kernel Scanner Project
<p>Kernel scanner is an attempt to scan the Linux kernel directory and divide all the existing functions into two distinct categories of safe and unsafe i.e. starting from a system call, the process will be preempted if it uses one of the unsafe system calls. </p>

## Prerequisites
<p>Python 2.7 or higher</p>
<p>cscope</p>
<!--
<p>coccinelle @ https://github.com/coccinelle/coccinelle</p> 
<p>cscope @ http://cscope.sourceforge.net/</p>
-->

## Download and extract kernel archive
<p>wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.8.2.tar.xz</p>
<p>xz -cd linux-4.8.2.tar.xz | tar xvf -</p>


## Install joblib for Python 3.4
<p>Joblib (Lightweight pipelining in Python) to enable parallel cscope scan
<p>wget https://bootstrap.pypa.io/get-pip.py</p>
<p>sudo python3.4 get-pip.py</p>
<p>rm get-pip.py -f</p>
<p>sudo pip install joblib</p>

## Looking for functions who call "schedule" syscall
<p>cscope -L -3 "schedule" | awk '{print $4 " "  $2 " "$1}' | sort</p>
<!--
## Scanning results for "schedule" syscall
<p>See the results at http://people.cs.pitt.edu/~hasanzadeh/pages/results.html</p>
<p>Forward Chaining time = 15.07 hours</p>
<p>Backward Chaining time = 1.37 hours</p>
-->

