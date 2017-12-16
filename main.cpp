/// general 
#include <iostream>
#include <mutex>

/// must be first 
#include <GL/glew.h>

/// window
#include "OpenGLWindow.h"

/// Opengl Drawing
#include "MainDrawFunc.h"

/// Error handling
#include "ErrorLogs.h"

int main(int argc, char const *argv[])
{	
	OpenGLWindow window(true); 
	MainDrawFunc drawing(window);

	window.startMainLoop(drawing);

	return 0;
}
