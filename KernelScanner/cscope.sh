#!/bin/bash
# Scanning the entire Linux kernel directory using cscope
# and generating a static database of functions
# (C) Mohammad H. Mofrad, 2016

TMP=tmp
if [ ! -d $TMP ];
then
     mkdir $TMP;
fi

LNX=~/linux-4.8.2

# Write only the files which are NOT symlinks
find $LNX \( \( -iname "*.c" -o -iname "*.cc" -o -iname "*.h" \) -and \( -not -type l \) \) -print > cscope.files
# Add the target of the symlink for all files matching the right extension, and are symlinks
find $LNX \( \( -iname "*.c" -o -iname "*.cc" -o -iname "*.h" \) -and -type l \) -printf "%l\n"  >> cscope.files

