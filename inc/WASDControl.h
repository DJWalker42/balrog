#ifndef DJW_WASD_CONTROL
#define DJW_WASD_CONTROL

#include <camControl.h>

/*
To set callbacks in glfw from class member functions those functions have to
be declared static. However, we want to be able to access data members of the
class and its base from the callback function. Thus we implement the Singleton
pattern in order to call a non-static member function from a single instance
of the class via a factory static member function. For performace reasons we
require that this factory function does not take any parameters. In otherwords
all data members of the class and its base have to be default initialised then
set to the required values using setter member functions (if the default values
are not adequate).
*/

class WASDCtrl : public camControl{
private:
	WASDCtrl();

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
		getInstance().framebufferSizeCallbackImpl(window, width, height);//defined in base class; do not shadow.
	}

	void setInitialXY (float x_value, float y_value) {
		if(!m_initialised){
			m_prev_X = x_value;
			m_prev_Y = y_value;
			m_initialised = true;
		} // else silently do nothing
	}

	//interface functions from camControl base class
	glm::mat4 calcModelTransform() override;
	void update() override;
	method whoAmI() const override { return method::WASD; }

	//WASD specfic getters
	float getMinFOV() const {return m_min_FOV;}
	float getMaxFOV() const {return m_max_FOV;}
	float getMinPitch() const {return m_min_pitch;}
	float getMaxPitch() const {return m_max_pitch;}
	float getMouseSenseX() const {return m_sensitivity_x;}
	float getMouseSenseY() const {return m_sensitivity_y;}

	//WASD specific setters
	void setMinFOV(float value_in_degs);
	void setMaxFOV(float value_in_degs);
	void setMinPitch(float value_in_rads);
	void setMaxPitch(float value_in_rads);
	void setMouseSenseX(float value);
	void setMouseSenseY(float value);

private:
	//callback implementations
	void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouseButtonCallbackImpl(GLFWwindow * window, int button, int action, int mods);
	void cursorCallbackImpl(GLFWwindow *window, double x, double y);
	void scrollCallbackImpl(GLFWwindow* window, double xoff, double yoff);

private:
	//data members
	//remembering internal state
	float m_prev_X;	//!< for cursor position computations
	float m_prev_Y;	//!< for cursor position computations
	float m_pitch;	//!< keeps track of the accumulated pitch angle
	bool m_initialised;

	//configurable values
	float m_min_FOV; //!< minimum field-of-view to reach to when scrolling
	float m_max_FOV; //!< maximum field-of-view to reach to when scrolling.

	float m_sensitivity_x; //!< mouse horizontal sensitivity (yaw rotations)
	float m_sensitivity_y; //!< mouse vertical sensitivity (pitch rotations)

	float m_max_pitch; //maximum pitch angle in radians
	float m_min_pitch;//minimum pitch angle in radians
};

#endif
