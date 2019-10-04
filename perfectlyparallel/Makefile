#!/usr/bin/make
# Makefile
# (c) Mohammad HMofrad, 2017
# (e) mohammad.hmofrad@pitt.edu

CC     := gcc
CFLAGS := -g -Wall -Werror -lm
PFLAGS := -lpthread

ANSI_RED   := \033[0;31m
ANSI_GREEN := \033[0;32m
ANSI_RESET := \033[0m

PAR_READ := parallel_read
SEQ_READ := sequential_read

install: 
	@echo -e "[Compiling]"
	$(CC) $(CFLAGS) $(PFLAGS) -o $(PAR_READ) $(PAR_READ).c
	$(CC) $(CFLAGS)           -o $(SEQ_READ) $(SEQ_READ).c
	@echo -e "$(ANSI_GREEN)Perfectly parallel cmpiled successfully$(ANSI_RESET)"    
clean:
	@echo -e "[Deleting]"    
	$(RM) $(PAR_READ)
	$(RM) $(SEQ_READ)    
	@echo -e "$(ANSI_RED)Perfectly parallel cleaned successfully$(ANSI_RESET)"    
