# balrog

## A developing C++ interface to OpenGL.

In order to use this code you will need the following:

* Graphics Library Mathematics (glm) - http://glm.g-truc.net/0.9.7/index.html
* Graphics Library Framework 3 (glfw3) - http://www.glfw.org/
* Simple OpenGL Image Loader 2 (SOIL2) - https://bitbucket.org/SpartanJ/soil2
* Asset Importer 3.x (assimp) - http://www.assimp.org/

glm is a header only library i.e. no compiling. For the other dependencies follow their respective instructions to download and build the libraries.

On Windows when building an opengl application/executable remember to link to the opengl32.lib as well as other dependencies.