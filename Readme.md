# balrog

## A developing C++ interface to OpenGL.

In order to use this code you will need the following:

* Graphics Library Mathematics (glm) - http://glm.g-truc.net/0.9.7/index.html
* Graphics Library Framework 3.x (glfw3) - http://www.glfw.org/
* Simple OpenGL Image Loader 2 (SOIL2) - https://bitbucket.org/SpartanJ/soil2
* Asset Importer 3.x (assimp) - http://www.assimp.org/

glm is a header only library i.e. no compiling. For the other dependencies follow their respective instructions to download and build the libraries. The Asset Importer library will build a dynamically linked library file as well as a static library file; you will need both in order to run the code in the example directory.

On Windows when building an opengl application/executable remember to link to the `opengl32.lib` as well as other dependencies.

The files `gl_core_4_x.c` and `gl_core_4_x.h` were generated using the Lua scripts provided by [OpenGL Loader Generator](https://bitbucket.org/alfonse/glloadgen/wiki/Home).
These files provide the means to load OpenGL API functions into C/C++ applications (alternative method to using glut or glew). The `initGraphics()` function in the Graphics.cpp source file does this loading for you.
The `gl_*` source and header files were generated using the command:

`lua LoadGen.lua -style=pointer_c -spec=gl -version=4.x -profile=core core_4_x`

where x is replaced by the minor version of OpenGL you want to target. The command above is translated as meaning that they target the core profile of OpenGL version 4.x using a function-pointer-based style for C. If you want a different OpenGL profile, version, or style go to website mentioned above and follow the instructions found there. Replace the relevant files with those that you generate. Note that the shader scripts will need modification to change the version and profile line at the top of the files.

Note that I got the following warning when compiling using the clang compiler on Mac OS X:  

* clang: warning: treating 'c' input as 'c++' when in C++ mode, this behavior is deprecated

This can be mitigated by generating the gl_core_4_x files using the pointer_cpp style option. However, this means that all the gl<function_name> functions require converting to the namespace version i.e gl::<function_name> in the source files (plus some other minor changes); very tedious, maybe leave for a rainy day.

The examples directory currently contains a single sample of how to use the library. You will need to link this sample with the output built from the source files, the assimp library, the glfw3 library, and the SOIL2 library. On Windows you will also need to link with opengl32.lib. You will also have to add assimp's dll to your path (or copy it to the executable directory). Note with recent changes I may have broken this - needs testing.

On the Mac OSX platform if using XCode you will have to add the OpenGL and CoreFoundation frameworks to the library inputs.

###Disclaimer:
The files contained in the resources directory are not of my creation. I obtained them from the samples directory of [MeshLab](http://meshlab.sourceforge.net/). Similarily, the data for the Sphere, Torus, and Teapot renderables were obtained via google searches.

####Why balrog?
I had been watching LOTR at the time of creating the repository - plus its got the letters 'o', 'g', and 'l'.
