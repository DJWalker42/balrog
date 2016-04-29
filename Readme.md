# balrog

## A developing C++ interface to OpenGL.

In order to use this code you will need the following:

* Graphics Library Mathematics (glm) - http://glm.g-truc.net/0.9.7/index.html
* Graphics Library Framework 3.x (glfw3) - http://www.glfw.org/
* Simple OpenGL Image Loader 2 (SOIL2) - https://bitbucket.org/SpartanJ/soil2
* Asset Importer 3.x (assimp) - http://www.assimp.org/

glm is a header only library i.e. no compiling. For the other dependencies follow their respective instructions to download and build the libraries. The Asset Importer library will build a dynamically linked library file as well as a static library file. 

On Windows when building an opengl application/executable remember to link to the `opengl32.lib` as well as other dependencies.

The files `gl_core_4_2.c` and `gl_core_4_2.h` were generated using the Lua scripts provided by OpenGL Loader Generator (https://bitbucket.org/alfonse/glloadgen/wiki/Home). 
These files provide the means to load OpenGL API functions into C/C++ applications (alternative method to using glut or glew). The `initGraphics()` function in the Graphics.cpp source file does this loading for you.
The `gl_*` source and header files were generated using the command:

`lua LoadGen.lua -style=pointer_c -spec=gl -version=4.2 -profile=core core_4_2` 

meaning that they target the core profile of OpenGL version 4.2 using a function-pointer-based style for C. If you want a different OpenGL profile, version, or style go to website mentioned above and follow the instructions found there. Replace the relevant files with those that you generate.

The examples directory currently contains a single sample of how to use the library. You will need to link this sample with the output built from the source files, the assimp library (dll or .lib), and the glfw3 library but not with SOIL2 as it doesn't use any functions from that library. On Windows you will also need to link with opengl32.lib. 

###Disclaimer:
The files contained in the resources directory are not of my creation. I obtained them from the samples directory of MeshLab (http://meshlab.sourceforge.net/). Similarily, the data for the Sphere, Torus, and Teapot renderables were obtained via google searches. 
