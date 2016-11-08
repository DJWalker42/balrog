#ifndef DJW_FREE_CAM_CONTROL_H
#define DJW_FREE_CAM_CONTROL_H

#include <camControl.h>
#include <array>

/*
To set callbacks in glfw from class member functions those functions have to
be declared static. However, we want to be able to access data members of the
class and its base from the callback function. Thus we implement the Singleton
pattern in order to call a non-static member function from a single instance
of the class via a factory static member function. For performace reasons we
require that this factory function does not take any parameters. In otherwords
all data members of the class and its base have to be default initialised then
set to the required values using setter member functions.
*/



class FreeCamCtrl : public camControl{
private:
	FreeCamCtrl();

	FreeCamCtrl(FreeCamCtrl const&);
	void operator=(FreeCamCtrl const&); //prevent copies

public:

	static FreeCamCtrl& getInstance()
	{
		static FreeCamCtrl s_FreeCamCtrl; //first call to getInstance creates an instance, subsequent calls just return the instance.
		return s_FreeCamCtrl;
	}

	//callbacks for input
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		getInstance().keyCallbackImpl(window, key, scancode, action, mods);
	}

	static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
	{
		getInstance().mouseButtonCallbackImpl(window, button, action, mods);
	}

	static void cursorCallback(GLFWwindow *window, double x, double y)
	{
		getInstance().cursorCallbackImpl(window, x, y);
	}

	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset){
		/* null body intentional */
	}

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		getInstance().framebufferSizeCallbackImpl(window, width, height);//defined in base class; do not shadow.
	}

	//interface functions from camControl base class
	glm::mat4 calcModelTransform() override;
	virtual void update() override;
	void initialise(GLFWwindow* glfw_win_ptr) override {/*null body intentional*/}
	method whoAmI() const override{ return method::FREE; }

private:
	//class implementations of the callbacks
	void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods);
	void cursorCallbackImpl(GLFWwindow *window, double x, double y);
};


#endif
