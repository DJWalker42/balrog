#ifndef DJW_CONTROL_H
#define DJW_CONTROL_H

#include <Camera.h>

#include <glm/glm.hpp>


namespace rot{
	enum {ROLL_P, ROLL_N, PITCH_P, PITCH_N, YAW_P, YAW_N};
}


namespace glfw{
	const int numOfKeys = 1024;
	const int numOfMouseButtons = 8;
}

class camControl
{
protected:
	camControl() :
		m_pCamera(nullptr),
		m_viewMatrix(),
		m_windowWidth(0),
		m_windowHeight(0),
		m_translateSpeed(),
		m_rotateSpeed()
	{
		for(int i = 0; i < glfw::numOfKeys; ++i){
			m_keys[i] = false;
		}
	}

public:
	virtual ~camControl() {}

public:
	enum method {ARC, FREE, WASD}; //add items for any more camera control methods here

public:

	virtual glm::mat4 calcModelTransform()= 0;
	virtual void update() = 0;

	virtual method whoAmI() const = 0;

	void registerCamera(Camera* pCamera)
	{
		if (pCamera){
			m_pCamera = pCamera;
			m_viewMatrix = m_pCamera->getViewMatrix();
			m_windowWidth = int (m_pCamera->getViewWidth());
			m_windowHeight = int (m_pCamera->getViewHeight());
		} else {
			throw std::runtime_error("Camera pointer is null\n\n");
		}
	}

	//this returns a copy of the data member pointer with pointer-to-const-object qualifier.
	//const Camera* getCameraPtr() const { return m_pCamera; }
	//this returns a const pointer to the Camera object.
	Camera* const & getCameraPtr() const { return m_pCamera; }

	void setRotateAngle(const glm::vec3& angle) { m_rotateSpeed = angle; }
	void setTranslateDistance(const glm::vec3& distance) { m_translateSpeed = distance; }

	glm::vec3 getRotateAngle() const { return m_rotateSpeed; }
	glm::vec3 getTranslateDistance() const { return m_translateSpeed; }

	void processKeys(GLFWwindow* win, int key, int action)
	{
		using namespace glfw;

		/* Allow user to close the window using an escape press */
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
			glfwSetWindowShouldClose(win, GL_TRUE);
		}

		if (key >= 0 && key < numOfKeys){
			m_keys[key] = (action == GLFW_PRESS || action == GLFW_REPEAT);
		}
	}

	void processMouse(int button, int action)
	{
		using namespace glfw;

		if (button >= 0 && button < numOfMouseButtons) {
			m_mouse[button] = (action == GLFW_PRESS);
		}
	}

protected:

	//Update relevant values on context window resize event - this is common to all camControl methods.
	void framebufferSizeCallbackImpl(GLFWwindow* window, int width, int height){
		//change the context window's attributes
		glViewport(0,0,width, height);

		//update Frustrum with new values for recalculation of the projection matrix
		m_pCamera->setViewWidth(static_cast<float>(width));
		m_pCamera->setViewHeight(static_cast<float>(height));

		//update camControl data members with the new values to get scaling correct.
		m_windowWidth = width;
		m_windowHeight = height;
	}

protected:
	Camera* m_pCamera;
	glm::mat4 m_viewMatrix;
	int m_windowWidth;
	int m_windowHeight;

	glm::vec3 m_translateSpeed;
	glm::vec3 m_rotateSpeed;

	bool m_keys[glfw::numOfKeys];
	bool m_mouse[glfw::numOfMouseButtons];


};


#endif
