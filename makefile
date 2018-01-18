CC = gcc
CXX = g++

DIRS = 	game									\
		sound									\
		time									\
		graphics								\
		graphics/procedural						\
		graphics/procedural/proceduralMap		\
		graphics/procedural/proceduralTexture	\

INCLUDE = $(patsubst %, -I%/, $(DIRS))

LINKS = -lX11 -lopenal -lGLEW -lGLU -lGL -lIL -lILU

window: clear main.cpp
	g++-7 -std=c++17 -I. $(INCLUDE) -DDEBUG_PRINT main.cpp ${LINKS} -o window
	./window 

clear: 
	rm -f window