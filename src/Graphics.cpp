#include <Graphics.h>

//This should be called only once. 


GLFWwindow* init_graphics(int w, int h, const std::string& title, bool full_screen)
{
	//initialisation stuff for glfw
	if( glfwInit() == GL_FALSE) {
		return nullptr;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	GLFWwindow* window = glfwCreateWindow(w, h, title.c_str(), full_screen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//intialise gl_loader AFTER making the opengl context current to a glfw window.
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwTerminate(); //we must terminate glfw if it has been initialised
		return nullptr;
	}

	return window;
}