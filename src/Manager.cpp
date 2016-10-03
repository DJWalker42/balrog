#include <Manager.h>
#include <Arcball.h>
#include <freeCamControl.h>
#include <WASDControl.h>

namespace app{

	Manager::Manager(GLFWwindow* win, IScene* scene, camControl& camCtrl, const glm::vec4& clear_colour) :
		p_glfw_win(win),
		p_scene(scene),
		m_camCtrl(camCtrl),
		m_clear_colour(clear_colour),
		m_base_translate(m_camCtrl.getTranslateDistance()),
		m_base_rotate(m_camCtrl.getRotateAngle())
	{
		switch(m_camCtrl.whoAmI()) {
		case camControl::method::ARC:

			glfwSetKeyCallback(p_glfw_win, &Arcball::keyCallback);
			glfwSetMouseButtonCallback(p_glfw_win, &Arcball::mouseButtonCallback);
			glfwSetCursorPosCallback(p_glfw_win, &Arcball::cursorCallback);
			glfwSetFramebufferSizeCallback(p_glfw_win, &Arcball::framebufferSizeCallback);

			break;
		case camControl::method::FREE:

			glfwSetKeyCallback(p_glfw_win, &FreeCamCtrl::keyCallback);
			glfwSetMouseButtonCallback(p_glfw_win, &FreeCamCtrl::mouseButtonCallback);
			glfwSetCursorPosCallback(p_glfw_win, &FreeCamCtrl::cursorCallback);
			glfwSetFramebufferSizeCallback(p_glfw_win, &FreeCamCtrl::framebufferSizeCallback);

			break;
		case camControl::method::WASD:

			glfwSetKeyCallback(p_glfw_win, &WASDCtrl::keyCallback);
			glfwSetMouseButtonCallback(p_glfw_win, &WASDCtrl::mouseButtonCallback);
			glfwSetScrollCallback(p_glfw_win, &WASDCtrl::scrollCallback);
			glfwSetCursorPosCallback(p_glfw_win, &WASDCtrl::cursorCallback);
			glfwSetFramebufferSizeCallback(p_glfw_win, &WASDCtrl::framebufferSizeCallback);

			double x, y;
			glfwGetCursorPos(p_glfw_win, &x, &y);
			static_cast<WASDCtrl&>(m_camCtrl).setInitialXY(
				m_camCtrl.getCameraPtr()->getViewWidth() / 2.f, float(y));
			break;

		default:
			//cannot be accessed
			break;
		}

		//set up the clear colour
		glClearColor(m_clear_colour[0], m_clear_colour[1], m_clear_colour[2], m_clear_colour[3]);

		p_scene->setCameraControl(&m_camCtrl);
		p_scene->initScene(p_glfw_win);


	}

	int Manager::run(){

		//used to implement a pause functionality
		int prevState = 0;
		bool pause = false;
		double prevTime = 0.;

		//main loop
		while (!glfwWindowShouldClose(p_glfw_win)) {
			glfwPollEvents(); //check for user interaction, key presses, mouse move, mouse button clicks, etc.

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			//check if user has pressed 'P'
			int state = glfwGetKey(p_glfw_win, GLFW_KEY_P);
			if (state == GLFW_PRESS && prevState != state) { //second condition avoids rapid toggling when pressing 'P'
				pause = !pause; //toggle the pause
			}
			prevState = state;

			//update the scene - typically processes the pause toggle, then does anything else you need - processing program logic,
			//applying updates to renderable objects, modifying shader variables, and so on. It is here we could apply some depth sorting
			//after the updates have been applied so that the renderable objects are draw from back to front in the view port e.g. using "oct-trees"
			double start = glfwGetTime();
			p_scene->updateScene(prevTime, start, pause, p_glfw_win);

			//Render the scene - typically gets and applies changes to the model, view, and projection matrices (and any associated matrices,
			//for example, the normal matrix) then calls the render member function for each object in the renderable list (vector).
			p_scene->drawScene();

			glfwSwapBuffers(p_glfw_win); //uses double buffering - front buffer is displayed while back buffer is being drawn, swaps buffers at screen refresh rate(?)


			//first attempt at normalising the "speed" of the application depending on the hardware on which it is run.
			double elapsed = glfwGetTime() - start;
			m_camCtrl.setTranslateDistance(m_base_translate * float(elapsed));

			if (m_camCtrl.whoAmI() != camControl::method::ARC ) {//the arcball becomes jittery otherwise
				m_camCtrl.setRotateAngle(m_base_rotate * float(elapsed));
			}

			prevTime = start;
		}

		//finished with glfw so terminate.
		glfwTerminate();
		return 0;
	}

}//app
