#!/usr/bin/make

# File: Makefile
# Author: Mohammad H. Mofrad
# Email: hasanzadeh@cs.pitt.edu
# Date: July 2016; Last revision 07-22-2016
# Copyright (C) Mohammad H. Mofrad - All Rights Reserved

CC := gcc
CFLAGS := -g -Wall
OBJ := hello
SRC := hello.c
$(OBJ): $(SRC)
	$(CC) $(LDFLAGS) -o $(OBJ) $(SRC) $(LDLIBS)
clean:
	rm -f $(OBJ)
