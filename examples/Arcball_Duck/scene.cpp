#include "scene.h"

/*	Note: These function are implemented for a simple example scene were we only
		have one shader program for the scene.
		In "real" applications there are likely to be multiple shader programs in
		use; the scene writer can make these virtual functions as complex as they
		like for their particular application. In this case it would be nice to
		provide some mechanism by which we can refer to specific shaders e.g. a
		string literal id, rather than remembering where in the container
		(m_shaderList) the shader is located.
*/


myScene::myScene(	const std::vector<Renderable*>& pRendObjs,
					GLSLProgram* pShader,
					camControl* pCamCtrl) :
					IScene(pRendObjs, pShader, pCamCtrl),
					m_accumulate(0.f)
{}

void myScene::initScene(GLFWwindow* window)
{
	//for the example scene there is only one shader program ....
	auto it_s = m_shaderList.begin();
	(*it_s)->enable();

	//initialise each renderable
	for (auto it_r = m_renderList.begin(); it_r != m_renderList.end(); ++it_r)
		(*it_r)->load(*it_s);

	//set up the uniforms in your specific GLSL Program.
	//In our example shaders their are two structures.
	//The first, called Light, has position and intensity data members,
	//and the second, called Material, has coefficients for diffuse, specular,
	//and ambient reflection properties, as well as a "shinyness" factor
	//(Phong model of shading).
	(*it_s)->setUniform("Light.Position", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	(*it_s)->setUniform("Light.Intensity", glm::vec3(1.0f, 1.0f, 1.0f));
	(*it_s)->setUniform("Material.Kd", 0.9f, 0.9f, 0.9f);
	(*it_s)->setUniform("Material.Ks", 0.5f, 0.5f, 0.5f);
	(*it_s)->setUniform("Material.Ka", 0.1f, 0.1f, 0.1f);
	(*it_s)->setUniform("Material.shine", 100.0f);
	//There is also a boolean value that can use this to switch between Phong and
	//Oren-Nayer shading models.
	//Phong is shiny, Oren-Nayer provides a more flat/ceramic shading.
	(*it_s)->setUniform("phongOren", GL_FALSE);

	//assume we have one texture for the renderable(s) called "BaseMap".
	//if we add more textures to a renderable then we can add their names here.
	std::vector<std::string> texNames = { "BaseMap"};//, "mixMap" };
	for (size_t i = 0; i < texNames.size(); ++i)
		(*it_s)->setUniform(texNames[i], static_cast<GLuint>(i));

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glEnable(GL_DEPTH_TEST);
}


void myScene::updateScene(double t0, double t1, bool pause, GLFWwindow* win)
{
	processPause(t1, pause);


	if(glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS)
		glEnable(GL_MULTISAMPLE);

	if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS)
		glDisable(GL_MULTISAMPLE);

	if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	auto it_s = m_shaderList.begin();

	if (glfwGetKey(win, GLFW_KEY_G) == GLFW_PRESS)
		(*it_s)->setUniform("phongOren", GL_TRUE);

	if (glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS)
		(*it_s)->setUniform("phongOren", GL_FALSE);
}
