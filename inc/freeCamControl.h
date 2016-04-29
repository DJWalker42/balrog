#ifndef DJW_FREE_CAM_CONTROL_H
#define DJW_FREE_CAM_CONTROL_H

#include <camControl.h>
#include <array>

class FreeCamCtrl : public camControl{
private:
	FreeCamCtrl(const glm::vec3& translateSpeeds = glm::vec3(1.f), const glm::vec3& rotateSpeeds = glm::vec3(1.f));

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

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		getInstance().framebufferSizeCallbackImpl(window, width, height);
	}

	//interface functions from camControl base class
	virtual glm::mat4 calcModelTransform();
	virtual void update();

private:
	//class impelmentations of the callbacks
	void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods);
	void cursorCallbackImpl(GLFWwindow *window, double x, double y);
	void framebufferSizeCallbackImpl(GLFWwindow* window, int width, int height);
};


#endif