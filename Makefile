
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
RES = windres
BIN = bin

# ============================  SDL LIBS  ======================================
L_SDLC = -IC:/Compiler/SDL/include/SDL2  
L_SDLL = $(GRAPHICS) -LC:/Compiler/SDL/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -lSDL2_image

# ==============================  MACROS  ======================================
CFLAGS = $(DEBUG) -Wall -std=c++11 -c
LFLAGS = $(DEBUG) -Wall
OBJS = $(BIN)/res.o $(BIN)/main.o

# ============================ RECEPIES ========================================

$(BIN)/main.o: main.cpp main.h
	$(CC) main.cpp -o $@ $(CFLAGS) $(L_SDLC)

$(BIN)/%.o: %.cpp
	$(CC) $^ -o $@ $(CFLAGS) $(L_SDLC)

$(BIN)/res.o: res.rc info.h
	$(RES) res.rc  $@
	
# Link
$(BINDIR)\main.exe: DEBUG = -g -DDEBUG
$(BINDIR)\main.exe: $(OBJS)
	$(CC) $^ -o $(BIN)/main.exe $(LFLAGS) $(L_SDLL)

# ============================= PHONY RECEPIES =================================
.PHONY: all
all: clean $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(L_SDLL) -o $(BIN)/final.exe

.PHONY: install
install: DEBUG = -O2 -s -DNDEBUG
install: GRAPHICS = -w -Wl,-subsystem,windows
install: all Runner.cpp $(BIN)/res.o
	$(CC) Runner.cpp $(BIN)/res.o -static -o Play.exe
	Play.exe
	
.PHONY: clean
clean:
	del $(BIN)\*.o
	del $(BIN)\*.exe