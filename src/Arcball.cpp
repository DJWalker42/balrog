#include <Arcball.h>
#include <algorithm> //std::min

/**
* Constructor.
* @param roll_speed the speed of rotation
* @param zoom_speed is the speed at which the camera zooms in/out
*/
Arcball::Arcball() :
	camControl(),
	m_clicked(false),
	m_dragging(false),
	m_zoom_pos(false),
	m_zoom_neg(false),
	m_angle(0.f),
	m_prevTrans(1.f),
	m_currTrans(1.f),
	m_camAxis(0.f, 1.f, 0.f)
{}

/**
* Convert the mouse cursor coordinate on the window (i.e. from (0,0) to (windowWidth, windowHeight))
* into normalised screen coordinate , i.e. (-1, -1) to (1, 1)
*/
glm::vec3 Arcball::toScreenCoord(double x, double y)
{
	glm::vec3 coord(0.0f);

	coord.x = (2.f * float(x) - m_windowWidth) / m_windowWidth;
	coord.y = -(2.f * float(y) - m_windowHeight) / m_windowHeight;

	/* Clamp cursor to border of the windows, comment these lines to allow rotation when cursor is outside the window */
	coord.x = glm::clamp(coord.x, -1.0f, 1.0f);
	coord.y = glm::clamp(coord.y, -1.0f, 1.0f);

	float length_squared = coord.x * coord.x + coord.y * coord.y;
	if (length_squared < 1.0) {
		coord.z = sqrt(1.f - length_squared);
	} else {
		coord = glm::normalize(coord);
	}

	return coord;
}

/**
* Check for any keys pressed
*/
void Arcball::keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	processKeys(window, key, action);

	m_zoom_pos = m_keys[GLFW_KEY_Z];
	m_zoom_neg = m_keys[GLFW_KEY_X];
}


/**
* Check whether we clicked in the window
*/
void Arcball::mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods)
{
	processMouse(button, action);

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	m_clicked = m_mouse[GLFW_MOUSE_BUTTON_LEFT];

	if(m_clicked){
		m_prevPos = toScreenCoord(x, y);
	} else {
		m_dragging = false;
		m_prevTrans = m_currTrans;
	}
}

/*
* Check if the mouse moved in the window
*/
void Arcball::cursorCallbackImpl(GLFWwindow *window, double x, double y)
{
	if (m_clicked) {
		m_dragging = true;

		m_currPos = toScreenCoord(x, y);

		/* Calculate the angle in radians, and clamp it between 0 and pi/2 */
		m_angle = acos(std::min(1.f, glm::dot(m_prevPos, m_currPos)));

		//if angle really small then m_camAxis will be close to zero and may cause issue where model disappears from screen
		if (m_angle > 0.01f ) {
			/* Cross product to get the rotation axis in camera coordinate */
			m_camAxis = glm::cross(m_prevPos, m_currPos);
		}
	} //else do nothing - mouse just moves over viewport
}

/*
	The transforms accumulate. A single click should not cause a model transform to be computed.
	Only compute a model transform when dragging i.e. left button down whilst moving mouse.
*/
glm::mat4 Arcball::calcModelTransform()
{
	if (m_dragging) {
		glm::vec3 axis = glm::inverse(glm::mat3(m_viewMatrix * m_prevTrans)) * m_camAxis;
		m_currTrans = m_prevTrans * glm::rotate(glm::degrees(m_angle) * m_rotateSpeed[0], axis);
	} else {
		m_currTrans = m_prevTrans;
	}

	return m_currTrans;
}

void Arcball::update()
{
	if (m_zoom_pos) {
		m_pCamera->translate(glm::vec3(0.f, 0.f, -m_translateSpeed[0]));
	}

	if (m_zoom_neg) {
		m_pCamera->translate(glm::vec3(0.f, 0.f, m_translateSpeed[0]));
	}
}
