#include <Manager.h>
#include <Arcball.h>
#include <freeCamControl.h>
#include <WASDControl.h>

namespace app{

	/*
		For general and demonstrative purposes we need instances of each camera control method for quick
		and convenient access to each. In a "real" application we'd pick just one camera control method.

		Specific camera control methods are implemented as Singleton Classes.

	*/
	namespace {
		Arcball& g_camCtrlArc = Arcball::getInstance();
		FreeCamCtrl& g_camCtrlFree = FreeCamCtrl::getInstance();
		WASDCtrl& g_camCtrlWASD = WASDCtrl::getInstance();

		GLFWwindow* g_window = nullptr;
		bool viewPortInit = false;
		camCtrl camCtrlChoice;

		glm::vec3 baseTranslateSpeed;
		glm::vec3 baseRotateSpeed;
	}

	void initViewPort(	GLFWwindow* window,
						Camera* pCamera,
						camCtrl whichCtrl,
						const glm::vec3& tSpeeds,
						const glm::vec3& rSpeeds)
	{
		switch(whichCtrl) {
		case ARC:
			camCtrlChoice = ARC;

			glfwSetKeyCallback(window, &Arcball::keyCallback);
			glfwSetMouseButtonCallback(window, &Arcball::mouseButtonCallback);
			glfwSetCursorPosCallback(window, &Arcball::cursorCallback);
			glfwSetFramebufferSizeCallback(window, &Arcball::framebufferSizeCallback);

			g_camCtrlArc.registerCamera(pCamera);
			g_camCtrlArc.setTranslateDistance(tSpeeds);
			g_camCtrlArc.setRotateAngle(rSpeeds);

			break;
		case FREE:
			camCtrlChoice = FREE;

			glfwSetKeyCallback(window, &FreeCamCtrl::keyCallback);
			glfwSetMouseButtonCallback(window, &FreeCamCtrl::mouseButtonCallback);
			glfwSetCursorPosCallback(window, &FreeCamCtrl::cursorCallback);
			glfwSetFramebufferSizeCallback(window, &FreeCamCtrl::framebufferSizeCallback);

			g_camCtrlFree.registerCamera(pCamera);
			g_camCtrlFree.setTranslateDistance(tSpeeds);
			g_camCtrlFree.setRotateAngle(rSpeeds);
			break;
		case WASD:
			camCtrlChoice = WASD;

			glfwSetKeyCallback(window, &WASDCtrl::keyCallback);
			glfwSetMouseButtonCallback(window, &WASDCtrl::mouseButtonCallback);
			glfwSetScrollCallback(window, &WASDCtrl::scrollCallback);
			glfwSetCursorPosCallback(window, &WASDCtrl::cursorCallback);
			glfwSetFramebufferSizeCallback(window, &WASDCtrl::framebufferSizeCallback);

			g_camCtrlWASD.registerCamera(pCamera);
			g_camCtrlWASD.setTranslateDistance(tSpeeds);
			g_camCtrlWASD.setRotateAngle(rSpeeds);

			double x, y;
			glfwGetCursorPos(window, &x, &y);
			g_camCtrlWASD.setInitialX(pCamera->getViewWidth() / 2.f);
			g_camCtrlWASD.setInitialY(float(y));
			break;
		}

		baseTranslateSpeed = tSpeeds;
		baseRotateSpeed = rSpeeds;

		g_window = window;
		viewPortInit = true;
	}

	/* Ou est la pScene? */

	void run(IScene* pScene, const glm::vec4& clear, GLFWwindow* window)
	{
		//set up the clear colour
		glClearColor(clear[0], clear[1], clear[2], clear[3]);

		//ensure we have a valid GLFWwidow pointer either set using initViewPort or passed in as an argument
		if (g_window == nullptr) {
			if(window){
				g_window = window;
			} else {
				std::cerr << "\nNo glfw window specified\n\n";
				glfwTerminate();
				return;
			}
		}

		//ensure we have a valid camControl pointer either set to one of the ones above
		// or set in the calling environment
		if (pScene->getCamCtrlPtr() == nullptr) {
			if(!viewPortInit) {
				std::cerr << "\nNo camera control method specified\n\n";
				glfwTerminate();
				return;
			}

			switch(camCtrlChoice){
			case ARC:
				pScene->setCameraControl(&g_camCtrlArc);
				break;
			case FREE:
				pScene->setCameraControl(&g_camCtrlFree);
				break;
			case WASD:
				pScene->setCameraControl(&g_camCtrlWASD);
				break;
			}
		}

		camControl* p_localCamCtrl = pScene->getCamCtrlPtr();

		//initialise the scene - user defined but typically consists of initialising each renderable's buffer data
		// setting up some constant variables/shader uniforms, enabling the opengl depth test, etc.
		pScene->initScene(g_window);

		//used to implement a pause functionality
		int prevState = 0;
		bool pause = false;
		double prevTime = 0.;

		//main loop
		while (!glfwWindowShouldClose(g_window)) {
			glfwPollEvents(); //check for user interaction, key presses, mouse move, mouse button clicks, etc.

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			//check if user has pressed 'P'
			int state = glfwGetKey(g_window, GLFW_KEY_P);
			if (state == GLFW_PRESS && prevState != state) { //second condition avoids rapid toggling when pressing 'P'
				pause = !pause; //toggle the pause
			}
			prevState = state;

			//update the scene - typically processes the pause toggle, then does anything else you need - processing program logic,
			//applying updates to renderable objects, modifying shader variables, and so on. It is here we could apply some depth sorting
			//after the updates have been applied so that the renderable objects are draw from back to front in the view port e.g. using "oct-trees"
			double start = glfwGetTime();
			pScene->updateScene(prevTime, start, pause, g_window);

			//Render the scene - typically gets and applies changes to the model, view, and projection matrices (and any associated matrices,
			//for example, the normal matrix) then calls the render member function for each object in the renderable list (vector).
			pScene->drawScene();

			glfwSwapBuffers(g_window); //uses double buffering - front buffer is displayed while back buffer is being drawn, swaps buffers at screen refresh rate(?)


			//first attempt at normalising the "speed" of the application depending on the hardware on which it is run.
			double elapsed = glfwGetTime() - start;
			p_localCamCtrl->setTranslateDistance(baseTranslateSpeed * float(elapsed));

			if (camCtrlChoice != ARC ) {//the arcball becomes jittery otherwise
				p_localCamCtrl->setRotateAngle(baseRotateSpeed * float(elapsed));
			}

			prevTime = start;
		}

		//finished with glfw so terminate.
		glfwTerminate();
	}

}
