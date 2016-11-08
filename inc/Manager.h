#ifndef DJW_MANAGER_H
#define DJW_MANAGER_H

#include <IScene.h>
/*
*	It would be more efficient here to forward declare these classes rather than
*	include their headers. You would then include the header file for the method
* you want in your application code after including this header.
*/
#include <Arcball.h>
#include <freeCamControl.h>
#include <WASDControl.h>
/****************************************************************************/

#include <GLFW/glfw3.h>

#include <type_traits> //for std::is_same

namespace app{

	//required template parameters are Arcball, freeCamControl, or WASDCtrl
	//i.e. anything that inherits from camControl.
	template<typename T>
	class ManagerT {
	public:
		ManagerT(GLFWwindow* win, IScene* scene, T& camCtrl, const glm::vec4& clear_colour) :
			p_glfw_win(win),
			p_scene(scene),
			m_camCtrl(camCtrl),
			m_clear_colour(clear_colour),
			m_base_translate(m_camCtrl.getTranslateDistance()),
			m_base_rotate(m_camCtrl.getRotateAngle())
		{
			static_assert(std::is_base_of<camControl, T>::value,
				"ManagerT template parameter has to be derived from camControl class\n");

			glfwSetKeyCallback(p_glfw_win, &T::keyCallback);
			glfwSetMouseButtonCallback(p_glfw_win, &T::mouseButtonCallback);
			glfwSetScrollCallback(p_glfw_win, &T::scrollCallback);
			glfwSetCursorPosCallback(p_glfw_win, &T::cursorCallback);
			glfwSetFramebufferSizeCallback(p_glfw_win, &T::framebufferSizeCallback);

			//do any initialisation that the control method requires.
			m_camCtrl.initialise(p_glfw_win);

			//set up the clear colour
			glClearColor(m_clear_colour[0], m_clear_colour[1], m_clear_colour[2], m_clear_colour[3]);

			p_scene->setCameraControl(&m_camCtrl);
			p_scene->initScene(p_glfw_win);
		}
	public:
		ManagerT(const ManagerT&) = delete;
		void operator=(const ManagerT&) = delete;

	public:

		//Note the run function has to be in the header due to template
		//This is okay as the run function will not require much modification

		int run(){

			//used to implement a pause functionality
			int prevState = 0;
			bool pause = false;
			double prevTime = 0.;

			//main loop
			while (!glfwWindowShouldClose(p_glfw_win)) {
				glfwPollEvents(); //check for user interaction, key presses, mouse move, mouse button clicks, etc.

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glEnable(GL_DEPTH_TEST); //ensure test is done in case state of GL has be changed elsewhere.

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

	private:
		GLFWwindow* p_glfw_win;
		IScene* p_scene;
		T& m_camCtrl;
		glm::vec4 m_clear_colour;
		const glm::vec3 m_base_translate;
		const glm::vec3 m_base_rotate;
	};



	class Manager {
	private:
		Manager(GLFWwindow* win, IScene* scene, camControl& camCtrl, const glm::vec4& clear_colour);

	public:
		Manager(const Manager&) = delete;
		void operator=(const Manager&) = delete;

	public:
		static Manager& getInstance(GLFWwindow* window, IScene* scene, camControl& cam_control, const glm::vec4& clear_colour = glm::vec4(0.f)){
			static Manager man(window, scene, cam_control, clear_colour);
			return man;
		}

		int run();

	private:
		GLFWwindow* p_glfw_win;
		IScene* p_scene;
		camControl& m_camCtrl;
		glm::vec4 m_clear_colour;
		const glm::vec3 m_base_translate;
		const glm::vec3 m_base_rotate;
	};

}

#endif
