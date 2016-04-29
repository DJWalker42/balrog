#ifndef DJW_MANAGER_H
#define DJW_MANAGER_H

#include <IScene.h>
#include <GLFW/glfw3.h>

namespace app{

	enum camCtrl { ARC, FREE, WASD };

	void initViewPort(	GLFWwindow* window, 
						Camera* pCamera, 
						camCtrl whichCtrl, 
						const glm::vec3& translateSpeeds = glm::vec3(.05f), 
						const glm::vec3& rotateSpeeds = glm::vec3(.05f));

	void run(IScene* pScene, const glm::vec4& clearColour = glm::vec4(0.f), GLFWwindow* window = nullptr );
}

#endif