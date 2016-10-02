#ifndef DJW_SCENE_H
#define DJW_SCENE_H

#include <vector>
#include <Renderable.h>
#include <camControl.h>

enum projection { FOV, ASPECT };


/*
	Interface base class for a "Scene". 

	You can derive your own "Scene" class from this interface and provide different definitions for
	initScene(), updateScene(), and drawScene().

	As designed IScene class consists of a vector of renderable objects, at least one complied and
	linked GLSLProgram used to actually render the Renderables, and a camControl 
	method to view and/or move around the scene.

	Note std::vector containers are used because we can dereference them by an integral index, meaning you can
	use, for example, an enum structure for convenience. Also we assume there won't be much inserting, removing, or
	sorting of the renderables or the shader programs. However, if this is the case it may be more efficient
	from a performance point-of-view to use std::list as the container. 
*/
class IScene{
public:
	IScene(	const std::vector<Renderable*>&  pRendObjs,
			GLSLProgram* pShader,
			camControl* pCamControl = nullptr);

	IScene(	const std::vector<Renderable*>&  pRendObjs = std::vector<Renderable*>(),
			const std::vector<GLSLProgram*>& pShaders = std::vector<GLSLProgram*>(),
			camControl* pCamControl = nullptr);
	
private://non-copyable
	IScene(const IScene&);
	void operator=(const IScene&);

public:
	virtual ~IScene(){}

	virtual void initScene(GLFWwindow* window);

	virtual void updateScene(double prevTime, double currTime, bool pause, GLFWwindow* window = nullptr);

	virtual void drawScene();

	void addRenerable(Renderable*);
	void addShader(GLSLProgram*);
	void removeRenderable(size_t idx);
	void setCameraControl(camControl* pCamCtrl) { m_pCamCtrl = pCamCtrl; }

	camControl* const & getCamCtrlPtr() const { return m_pCamCtrl; }

protected:
	/*
		Here we implement a pause functionality.
		To use the pause simply call this from within your updateScene() definition.
		The time, t, should be computed from glfwGetTime(), and you should toggle pause
		based on a key press (In the Manager.cpp source file this is done using the key 'P'). 
	*/
	void processPause(double t, bool pause);

protected:
	std::vector<Renderable*> m_renderList;
	std::vector<GLSLProgram*> m_shaderList;
	camControl* m_pCamCtrl;

	GLFWwindow* m_pWindow;

	//The following is needed to remember the paused state, which is reinstated on an unpause event
	std::vector<glm::mat4> m_prevModelMats;
	bool m_paused;							 
	double m_pauseTime;
};
#endif
