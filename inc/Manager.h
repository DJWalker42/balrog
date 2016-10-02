#ifndef DJW_MANAGER_H
#define DJW_MANAGER_H

#include <IScene.h>
#include <GLFW/glfw3.h>

namespace app{

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
