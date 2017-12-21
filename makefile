CC = gcc
CXX = g++

DIRS = game time graphics graphics/procedural/proceduralMap graphics/procedural/proceduralTexture
INCLUDE = $(patsubst %, -I%/, $(DIRS))

window: clear main.cpp
	g++-7 -O3 -std=c++17 -I. $(INCLUDE) -DDEBUG_PRINT main.cpp -lX11 -lGLEW -lGLU -lGL -lIL -lILU -o window
	./window 

clear: 
	rm -f window