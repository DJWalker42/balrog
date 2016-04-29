#include <freeCamControl.h>

FreeCamCtrl::FreeCamCtrl(const glm::vec3& transSpeed, const glm::vec3& rotSpeed) :
						camControl(transSpeed, rotSpeed)
{}


void FreeCamCtrl::keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	processKeys(window, key, action);
}

void FreeCamCtrl::mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods)
{}

void FreeCamCtrl::cursorCallbackImpl(GLFWwindow *window, double x, double y)
{}

void FreeCamCtrl::framebufferSizeCallbackImpl(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	//pass new values to the camera's frustrum to update projection matrix
	m_pCamera->setViewWidth(static_cast<float>(width));
	m_pCamera->setViewHeight(static_cast<float>(height));
}

glm::mat4 FreeCamCtrl::calcModelTransform()
{
	return glm::mat4(1.f);
}


void FreeCamCtrl::update()
{
	if (m_keys[GLFW_KEY_W]){
		m_pCamera->translateLocal(glm::vec3(0.f, 0.f, m_translateSpeed.z));
	}

	if (m_keys[GLFW_KEY_S]){
		m_pCamera->translateLocal(glm::vec3(0.f, 0.f, -m_translateSpeed.z));
	}

	if (m_keys[GLFW_KEY_Q]){
		m_pCamera->translateLocal(glm::vec3(-m_translateSpeed.x, 0.f, 0.f));
	}

	if (m_keys[GLFW_KEY_E]){
		m_pCamera->translateLocal(glm::vec3(m_translateSpeed.x, 0.f, 0.f));
	}

	if (m_keys[GLFW_KEY_UP]){
		m_pCamera->translateLocal(glm::vec3(0.f, m_translateSpeed.y, 0.f));
	}

	if (m_keys[GLFW_KEY_DOWN]){
		m_pCamera->translateLocal(glm::vec3(0.f, -m_translateSpeed.y, 0.f));
	}

	if (m_keys[GLFW_KEY_LEFT]){
		m_pCamera->roll(m_rotateSpeed.x);
	}

	if (m_keys[GLFW_KEY_RIGHT]){
		m_pCamera->roll(-m_rotateSpeed.x);
	}

	if (m_keys[GLFW_KEY_PAGE_UP]){
		m_pCamera->pitch(m_rotateSpeed.y);
	}

	if (m_keys[GLFW_KEY_PAGE_DOWN]){
		m_pCamera->pitch(-m_rotateSpeed.y);
	}

	if (m_keys[GLFW_KEY_A]){
		m_pCamera->yaw(m_rotateSpeed.z);
	}

	if (m_keys[GLFW_KEY_D]){
		m_pCamera->yaw(-m_rotateSpeed.z);
	}
}