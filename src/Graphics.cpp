#include <Graphics.h>
#include <iostream>

//This should be called only once. 


GLFWwindow* init_graphics(int w, int h, const std::string& title, bool full_screen)
{
	//initialisation stuff for glfw
	if( glfwInit() == GL_FALSE) {
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(w, h, title.c_str(), full_screen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
	
	if (window == nullptr) {
		std::cout << "glfw fail" << std::endl;
	}

	glfwMakeContextCurrent(window);

	//intialise gl_loader AFTER making the opengl context current to a glfw window.
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwTerminate(); //we must terminate glfw if it has been initialised
		std::cout << "Loader fail" << std::endl;
		return nullptr;
	}

	return window;
}