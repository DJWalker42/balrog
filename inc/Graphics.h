#ifndef GRAPHICS_LIBRARY_H
#define GRAPHICS_LIBRARY_H

/*
	To use these files you will need to get GLFW3 here http://www.glfw.org/docs/latest/
	The header is included in the repository but you will need to build a binary .lib file. Use CMake to generate the GLFW project and build. 

	You will also need OpenGL Loader Generator from here https://bitbucket.org/alfonse/glloadgen/wiki/Home this requires Lua to "install". 
	OpenGL Loader Generator produces two files a header and a c source that contain all the core functions and extensions required by OpenGL.
	I've included a version of these in the repository namely gl_core_4_2.h and .c. Thes files can be added to your project or built into
	a custom library to get access to OpenGL. This is an alternative to glut, freeglut, and/or glew. 

*/

#include <gl_core_4_2.h>
#include <GLFW/glfw3.h>
#include <string>

GLFWwindow* init_graphics(int width, int height, const std::string& title, bool full_screen = false);


#endif