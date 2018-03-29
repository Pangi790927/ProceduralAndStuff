CC = gcc
CXX = g++

DIRS = 	main									\
		sound									\
		time									\
		collisions								\
		graphics								\
		graphics/procedural						\
		graphics/procedural/proceduralMap		\
		graphics/procedural/proceduralTexture	\

INCLUDE = $(patsubst %, -I%/, $(DIRS))

LINKS = -lX11 -lopenal -lGLEW -lGLU -lGL -lIL -lILU

window: clear main.cpp
	g++-7 -O3 -std=c++17 -I. $(INCLUDE) -DDEBUG_PRINT main.cpp ${LINKS} -o window
	./window 

clear: 
	rm -f window