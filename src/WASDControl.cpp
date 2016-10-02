#include <WASDControl.h>
//#include <gl_core_4_1.h>

//These hard coded file scoped variables could be added as variables to the constructor

static const float FOVmin = 10.f;
static const float FOVmax = 60.f;

static float sensitivity_x = 0.005f;
static float sensitivity_y = 0.005f;

static float maxPitchAngle = .75f;
static float minPitchAngle = -.25f;

WASDCtrl::WASDCtrl() :
					camControl(),
					m_prevX(0.f),
					m_prevY(0.f),
					m_pitch(0.f)
{}


void WASDCtrl::keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	processKeys(window, key, action);
}

void WASDCtrl::mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods)
{}

void WASDCtrl::cursorCallbackImpl(GLFWwindow* window, double x, double y)
{
	float xOffset = m_prevX - float(x);
	float yOffset = m_prevY - float(y);

	xOffset *= sensitivity_x;
	yOffset *= sensitivity_y;

	m_pitch += yOffset;

	if (m_pitch > maxPitchAngle){
		m_pitch = maxPitchAngle;
		yOffset = 0.f;
	}

	if (m_pitch < minPitchAngle){
		m_pitch = minPitchAngle;
		yOffset = 0.f;
	}

	m_pCamera->yaw(xOffset);
	m_pCamera->firstPersonlookAt(yOffset);

	m_prevX = float(x);
	m_prevY = float(y);
}

void WASDCtrl::scrollCallbackImpl(GLFWwindow* window, double xOff, double yOff)
{
	float FOV = m_pCamera->getFieldOfViewY();

	if ( FOV >= FOVmin && FOV <= FOVmax){
		FOV -= static_cast<float>(yOff);
	}

	if (FOV < FOVmin){
		FOV = FOVmin;
	}

	if (FOV > FOVmax ){
		FOV = FOVmax;
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
