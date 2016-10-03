#include <WASDControl.h>
//#include <gl_core_4_1.h>

//These hard coded file scoped variables could be added as variables to the constructor

WASDCtrl::WASDCtrl() :
					camControl(),
					m_prev_X(0.f),
					m_prev_Y(0.f),
					m_pitch(0.f),
					m_initialised(false),
					m_min_FOV(10.f),
					m_max_FOV(60.f),
					m_sensitivity_x(0.005f),
					m_sensitivity_y(0.005f),
					m_max_pitch(.75f),
					m_min_pitch(-.25f)
{}


void WASDCtrl::keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	processKeys(window, key, action);
}

void WASDCtrl::mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods)
{}

void WASDCtrl::cursorCallbackImpl(GLFWwindow* window, double x, double y)
{
	float xOffset = m_prev_X - float(x);
	float yOffset = m_prev_Y - float(y);

	xOffset *= m_sensitivity_x;
	yOffset *= m_sensitivity_y;

	m_pitch += yOffset;

	if (m_pitch > m_max_pitch){
		m_pitch = m_max_pitch;
		yOffset = 0.f;
	}

	if (m_pitch < m_min_pitch){
		m_pitch = m_min_pitch;
		yOffset = 0.f;
	}

	m_pCamera->yaw(xOffset);
	m_pCamera->firstPersonlookAt(yOffset);

	m_prev_X = float(x);
	m_prev_Y = float(y);
}

void WASDCtrl::scrollCallbackImpl(GLFWwindow* window, double xOff, double yOff)
{
	float FOV = m_pCamera->getFieldOfViewY();

	if ( FOV >= m_min_FOV && FOV <= m_max_FOV){
		FOV -= static_cast<float>(yOff);
	}

	if (FOV < m_min_FOV){
		FOV = m_min_FOV;
	}

	if (FOV > m_max_FOV ){
		FOV = m_max_FOV;
	}

	m_pCamera->setFieldOfViewY(FOV);
}

glm::mat4 WASDCtrl::calcModelTransform()
{
	return glm::mat4(1.f);
}


void WASDCtrl::update()
{
	if (m_keys[GLFW_KEY_W]){
		m_pCamera->translateLocal(0.f, 0.f, m_translateSpeed.z);
	}
	if (m_keys[GLFW_KEY_S]){
		m_pCamera->translateLocal(0.f, 0.f, -m_translateSpeed.z);
	}
	if (m_keys[GLFW_KEY_A]){
		m_pCamera->yaw(m_rotateSpeed.x);
	}
	if (m_keys[GLFW_KEY_D]){
		m_pCamera->yaw(-m_rotateSpeed.x);
	}
	if (m_keys[GLFW_KEY_E]){
		m_pCamera->translateLocal(-m_translateSpeed.x, 0.f, 0.f);
	}
	if (m_keys[GLFW_KEY_Q]){
		m_pCamera->translateLocal(m_translateSpeed.x, 0.f, 0.f);
	}
	if (m_keys[GLFW_KEY_APOSTROPHE]){
		m_pitch = 0.f;
	}
}
