#include <iostream>

#include <Graphics.h>
#include <Manager.h>
#include <BasicMesh.h>
#include <Arcball.h>
#include "scene.h"

/*
	Note: There was a bug in the arcball code whereby clicking on another window then clicking
	back into the context window occasionally made the model disappear from the view port.
	This was seemingly caused by the current clicked position being very close to or the same as
	the previous clicked position, and that clicking back into the context window is seen as dragging.
	This leads to a very small or zero angle bewteen positions and an almost zero or zero camera axis when computed.
	This inturn makes the transform calculation go wonky.
	To avoid this I've but a conditional around the camera axis computation such that it
	won't be performed for angles smaller than .01 radians (see Arcball.cpp:Ln 93).
	I have yet to see the bug reappear. Removing the conditional makes the bug easy to induce.
*/

//put varibles here we might want to change
namespace{
	// -- change the root path to where you have saved the repository.
    std::string root = "/Users/mbaxhdw4/Play";
    
	std::string vertexShader(root + "/balrog/shaders/basicVertex.vert");
	std::string fragmentShader(root + "/balrog/shaders/PhongOren.frag");
	std::string meshModelData(root + "/balrog/resource/duck_triangulate.dae");

	//window stuff
	std::string winTitle("Duck, Fuzzy?");
	const int winWidth = 800;
	const int winHeight = 600;

	//camera/frustrum settings
	float vFOV = 40.f;
	float nearZ = .1f;
	float farZ = 1000.f;
	glm::vec3 eye(0.f, 0.f, 50.f), target(0.f, 0.f, 0.f), up(0.f, 1.f, 0.f);

	//camera control settings - increase values for faster screen movement
	glm::vec3 translateSpeed(10.f); //for zooming
	glm::vec3 rotateSpeed(.05f); // for manipulation of the arcball with the mouse.

	//mesh model matrix - here we fiddle with the scale and position to get the model central in the viewport.
    glm::mat4 meshModelMat = glm::translate( glm::scale( glm::mat4(), glm::vec3(.01f) ), glm::vec3(0.f, -300.f, 0.f) );

	//clear/background colour for window
	glm::vec4 clearColour(.2f);
    
}

void printInstructions(){
	std::cout << "Instructions\n\n";
	std::cout << "Use the mouse to click and drag\n\n";
	std::cout << "Use the following keys for different effects:\n";
	std::cout << "[1] Wire mesh\n";
	std::cout << "[2] Normal mesh\n";
	std::cout << "[z] Zoom in\n";
	std::cout << "[x] Zoom out\n";
	std::cout << "[v] Multisampling (you may have to zoom in to see the effect)\n";
	std::cout << "[b] No multismapling\n";
	std::cout << "[g] (Blinn)-Phong model shading\n";
	std::cout << "[h] Oren-Nayer model shading (this is the shading on start-up)\n";
	std::cout << "[Esc] Quit" << std::endl;
}

int main(){
	//initialise a context window and load opengl functions
	GLFWwindow* window = init_graphics(winWidth, winHeight, winTitle);

	//initialise a camera object - final argument gives the camera forward vector
	Camera camera(vFOV, float(winWidth), float(winHeight), nearZ, farZ, eye, up, target - eye);
    
    //Texture tex(GL_TEXTURE_2D, texLoc);
    //load the mesh model to a BasicMesh object
    BasicMesh mesh(meshModelMat, meshModelData);
    //Cube mesh(meshModelMat, &tex);
    
    //create a renderable list and add the (only) mesh model.
    std::vector<Renderable*> rendList(1, &mesh);

	//create (compile and link) a shader program from the shader scripts.
	GLSLProgram shader(vertexShader, fragmentShader);

	//create the scene from the renderable list and our compiled and linked shader.
	myScene scene(rendList, &shader);
    
    Arcball& arcball = Arcball::getInstance();
    arcball.setTranslateDistance(translateSpeed);
    arcball.setRotateAngle(rotateSpeed);
    arcball.registerCamera(&camera);
    
    //print instructions and run the scene
    printInstructions();
    
    app::Manager& exec = app::Manager::getInstance(window, &scene, arcball, clearColour);

	return exec.run();
}
