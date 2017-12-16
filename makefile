CC = gcc
CXX = g++

DIRS = game time graphics graphics/procedural
INCLUDE = $(patsubst %, -I%/, $(DIRS))

window: clear main.cpp
	g++-7 -std=c++17 -I. $(INCLUDE) -DDEBUG_PRINT main.cpp -lX11 -lGLEW -lGLU -lGL -lIL -lILU -o window
	./window 

clear: 
	rm -f window