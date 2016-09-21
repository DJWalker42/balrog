#ifndef DJW_ARCBALL_HPP
#define DJW_ARCBALL_HPP

#include <iostream>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <camControl.h>


class Arcball : public camControl{
//implement Arcball as a singleton class
private:
	Arcball(GLfloat roll_speed = 1.f, GLfloat zoom_speed = 1.f); 

	Arcball(Arcball const&);
	void operator=(Arcball const&); //prevent copies

public:

	static Arcball& getInstance() 
	{
		static Arcball s_Arcball; //first call to getInstance creates the Arcball, subsequent (internal) calls just return the instance.
		return s_Arcball;
	}

	//callbacks for GLFW
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
	glm::vec3 toScreenCoord(double x, double y);

	//callbacks implementations
	void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods);
	void cursorCallbackImpl(GLFWwindow *window, double x, double y);
	void framebufferSizeCallbackImpl(GLFWwindow* window, int width, int height);

private:
	//state data
	bool m_clicked;				//!< has the mouse button been pressed?
	bool m_dragging;			//!< has the mouse moved with the button pressed? 
	bool m_zoom_pos;			//!< zooming in or not
	bool m_zoom_neg;			//!< zooming out or not


	//internal data
	GLfloat m_angle;			//!< angle between mouse location and origin
	glm::mat4 m_prevTrans;		//!< the previous transformation 
	glm::mat4 m_currTrans;		//!< the current transformation - accumulates from previous transformations

	glm::vec3 m_prevPos;		//!< mouse previous position (x,y,z)
	glm::vec3 m_currPos;		//!< mouse current position (x,y,z)
	glm::vec3 m_camAxis;		//!< the axis of rotation (cross product of previous postion with current position)
};

#endif