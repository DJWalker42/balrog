#ifndef DJW_CONTROL_H
#define DJW_CONTROL_H

#include <glm/glm.hpp>
#include <Camera.hpp>

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
	camControl(const glm::vec3& translateSpeed, const glm::vec3& rotateSpeed) : 
		m_pCamera(nullptr),
		m_viewMatrix(),
		m_windowWidth(0),
		m_windowHeight(0),
		m_translateSpeed(translateSpeed),
		m_rotateSpeed(rotateSpeed)
	{
		for(int i = 0; i < glfw::numOfKeys; ++i){
			m_keys[i] = false;	
		}
	}

public:
	virtual ~camControl() {} 

public:

	virtual glm::mat4 calcModelTransform()= 0;
	virtual void update() = 0;

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
	const Camera* getCameraPtr() const { return m_pCamera; }
	//Camera* const & getCameraPtr() const { return m_pCamera; } //this would return a const-pointer-to-object

	void setRotateAngle(const glm::vec3& angle) { m_rotateSpeed = angle; }
	void setTranslateDistance(const glm::vec3& distance) { m_translateSpeed = distance; }

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