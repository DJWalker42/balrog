#ifndef DJW_WASD_CONTROL
#define DJW_WASD_CONTROL

#include <camControl.h>

class WASDCtrl : public camControl{
private:
	WASDCtrl(	const glm::vec3& transSpeed = glm::vec3(1.f), 
				const glm::vec3& rotateSpeed = glm::vec3(1.f));

	WASDCtrl(WASDCtrl const&);
	void operator=(WASDCtrl const&);

public:

	static WASDCtrl& getInstance()
	{
		static WASDCtrl s_WASDCtrl;
		return s_WASDCtrl;
	}

	//callbacks for input
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		getInstance().keyCallbackImpl(window, key, scancode, action, mods);
	}

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		getInstance().mouseButtonCallbackImpl(window, button, action, mods);
	}

	static void cursorCallback(GLFWwindow* window, double x, double y)
	{
		getInstance().cursorCallbackImpl(window, x, y);
	}

	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		getInstance().scrollCallbackImpl(window, xOffset, yOffset);
	}

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		getInstance().framebufferSizeCallbackImpl(window, width, height);
	}

	void setInitialX (float value) { m_prevX = value; }
	void setInitialY(float value) { m_prevY = value; }

	//interface functions from camControl base class
	virtual glm::mat4 calcModelTransform();
	virtual void update();

private:
	//callback implementations
	void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods);
	void cursorCallbackImpl(GLFWwindow *window, double x, double y);
	void scrollCallbackImpl(GLFWwindow* window, double xoff, double yoff);
	void framebufferSizeCallbackImpl(GLFWwindow* window, int width, int height);

private: 
	//data members
	float m_prevX;
	float m_prevY;
	float m_pitch;		//!< keeps track of the accumulated pitch angle
}; 

#endif