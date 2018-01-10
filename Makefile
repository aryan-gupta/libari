
# Copyright (c) 2017 The Gupta Empire - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
#
# Written by Aryan Gupta <me@theguptaempire.net>
# 
# ==============================================================================
# @author 			Aryan Gupta
# @project 			
# @file 			Makefile
# @date				 (YYYY-MM-DD)
# @version			1.0.0
# @brief 			
# ==============================================================================
.DEFAULT_GOAL := all
# ==========================  CONST MACROS  ====================================
CC = g++
BIN = bin

# ==============================  MACROS  ======================================
STD    = -std=c++17
CFLAGS = -Wall $(STD) -g -O0
LFLAGS = -Wall

# ============================ RECEPIES ========================================

any : test/main-any.cpp include/any.hpp
	g++ $(CFLAGS) -o bin/any.exe $<
	
heap : test/main-heap.cpp include/priority_queue.hpp
	g++ $(CFLAGS) -o bin/heap.exe $<

# ============================= PHONY RECEPIES =================================
.PHONY: all
all: clean $(TESTS)
	
.PHONY: clean
clean:
	del $(BIN)\*.o
	del $(BIN)\*.exe