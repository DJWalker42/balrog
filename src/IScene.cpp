#include <IScene.h>

IScene::IScene(	const std::vector<Renderable*>& renderList,
				GLSLProgram* pShader,
				camControl* pCamControl	) :
				m_renderList(renderList),
				m_shaderList(pShader ? std::vector<GLSLProgram*>(1, pShader) : std::vector<GLSLProgram*>()),
				m_pCamCtrl(pCamControl),
				m_pWindow(nullptr),
				m_prevModelMats(m_renderList.size()),
				m_paused(false),
				m_pauseTime(0.0)
{}

IScene::IScene(	const std::vector<Renderable*>& renderList,
				const std::vector<GLSLProgram*>& pShaders,
				camControl* pCamControl) :
				m_renderList(renderList),
				m_shaderList(pShaders),
				m_pCamCtrl(pCamControl),
				m_pWindow(nullptr),
				m_prevModelMats(m_renderList.size()),
				m_paused(false),
				m_pauseTime(0.0)
{}



void IScene::addRenerable(Renderable* pRenderObject)
{
	m_renderList.push_back(pRenderObject);
}


void IScene::addShader(GLSLProgram* pShader)
{
	m_shaderList.push_back(pShader);
}


void IScene::removeRenderable(size_t idx)
{
#if _DEBUG
	assert(idx < m_renderList.size());
#endif
	auto it = m_renderList.begin();
	std::advance(it, idx);
	m_renderList.erase(it);
}

void IScene::processPause(double t, bool paused)
{
	if (paused && !m_paused) //on pause
	{
		int i = 0;
		for (auto it = m_renderList.begin(); it != m_renderList.end(); ++it)
			m_prevModelMats[i++] = (*it)->getModelAnimation();
		m_paused = true;
		m_pauseTime = t; //store the time at pause
	}

	if (m_paused && !paused) //on unpause
	{
		m_paused = false;
		glfwSetTime(m_pauseTime); //reset the time to the time when paused.
	}
}


void IScene::initScene(GLFWwindow* window)
{
	//for our "default scene" assume only one shader program that renders (e.g. consists of a vertex and fragment shaders)
	auto it_s = m_shaderList.begin();

	//initialise each renderable (the load function bind the vertex attributes to the opengl buffer arrays)
	for (auto it_r = m_renderList.begin(); it_r != m_renderList.end(); ++it_r){
		(*it_r)->load(*it_s);
	}

	//set up the uniforms in your specific GLSL Program.
	//he we assume there are two structures the first called Light that has position and intensity data members,
	//and the second called Material that has coefficients for diffuse, specular, and ambient reflection properties, as
	//well as a shinyness factor (i.e. a Phong model of shading).
	(*it_s)->setUniform("Light.Position", glm::vec4(0.0f, 1.0f, 5.0f, 1.0f));
	(*it_s)->setUniform("Light.Intensity", glm::vec3(10.0f, 10.0f, 10.0f));
	(*it_s)->setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
	(*it_s)->setUniform("Material.Ks", 0.95f, 0.95f, 0.95f);
	(*it_s)->setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
	(*it_s)->setUniform("Material.shine", 100.0f);

	//we also assume each renderable is to be rendered with a mixture of two textures "BaseMap" and "mixMap"
	//e.g. a brick texture covered by a moss texture.
	//set the binding locations for the textures
	std::vector<std::string> texNames = { "BaseMap"};//, "mixMap" };
	for (size_t i = 0; i < texNames.size(); ++i){
		(*it_s)->setUniform(texNames[i], static_cast<GLuint>(i));
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glEnable(GL_MULTISAMPLE); //Anti-aliasing using hardware
	glEnable(GL_DEPTH_TEST);
}


void IScene::updateScene(double t0, double t1, bool paused, GLFWwindow* win)
{
	processPause(t1, paused); //member function from base class IScene
}

/*
Here we assume the entire scene is rendered using just one GLSLProgram, and that program
has a modelView matrix, a normal matrix, and a modelViewProjection(mvp) matrix.
*/
void IScene::drawScene()
{
	auto it_s = m_shaderList.begin(); //here we assume we only have one shader

	m_pCamCtrl->update(); //makes changes to the view matrix depending on user input (key press, mouse button press, mouse move, window resize ...)

	//normally the projection matrix remains constant throughout but here we take account of any potential window resizes or changes to the FOV
	glm::mat4 proj = m_pCamCtrl->getCameraPtr()->getProjectionMatrix();
	glm::mat4 view = m_pCamCtrl->getCameraPtr()->getViewMatrix() * m_pCamCtrl->calcModelTransform();

	int i = 0;
	for (auto it_r = m_renderList.begin(); it_r != m_renderList.end(); ++it_r){
		glm::mat4 model = m_paused ? m_prevModelMats[i++] : (*it_r)->getModelAnimation();
		glm::mat4 mv = view * model;

		(*it_s)->setUniform("modelView", mv);
		(*it_s)->setUniform("normMat", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
		(*it_s)->setUniform("mvp", proj * mv);

		(*it_r)->render();
	}
}
