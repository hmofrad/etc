#!/bin/bash

# File: Automake Compile On Write (ACOW)
# Author: Mohammad H. Mofrad
# Email: hasanzadeh@cs.pitt.edu
# Date: July 2016; Last revision 07-22-2016
# Copyright (C) Mohammad H. Mofrad - All Rights Reserved

# A shell script for automating the process of
# compiling and building a C program upon a change
# It accepts a C program along with
# the name of the binary file from the make
# or another compiler tool and finally
# try to execute the created binary file

# GNU stat is used to detect the file changes using
# "stat --format %Z FILE" which
# determines the time of last change of FILE
# as seconds since Epoch

src=$1
bin=$2
cmd=$3
sec=$4

if  [ ! -f "$src" ] || [ -z "$bin" ] || [ -z "$cmd" ] || [ -z "$sec" ]; then
    echo "Usage: $0 src dst cmd secs"
    exit
fi

echo "Source c file:           $src"
echo "Destination binary file: $bin"
echo "Command to execute:      $cmd"
echo "Referesh interval:       $sec"

$cmd
./$bin

while true; do
    t0=`stat --format %Z $src`
         sleep $sec
    t1=`stat --format %Z $src`
    if [ "$t1" -gt "$t0" ]; then
        echo "$src has been changned!"
        $cmd
        ./$bin
    fi
done
