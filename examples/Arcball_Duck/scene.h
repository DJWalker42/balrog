#ifndef DJW_MYSCENE_H
#define DJW_MYSCENE_H

#include <IScene.h>

class myScene : public IScene
{
public:
	myScene(const std::vector<Renderable*>& pRendObjs,
			GLSLProgram* pShader,
			camControl* pCamControl = nullptr);

	virtual void initScene(GLFWwindow* window);
	virtual void updateScene(double prevTime, double currTime, bool pause, GLFWwindow* window);
private:
	glm::vec3 m_accumulate; 
};

#endif