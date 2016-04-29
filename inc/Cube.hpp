#ifndef CUBE_HPP
#define CUBE_HPP

#include <gl_core_4_2.h>
#include <glm/glm.hpp>
#include <GLSLProgram.hpp>
#include <Texture.hpp>
#include <Renderable.h>
#include <array>

namespace drw{
	enum face { FRONT, TOP, BACK, BOTTOM, LEFT, RIGHT };
}

class Cube : public Renderable{
public:
	Cube(	const glm::mat4& intialModel, 
			Texture* pTexture = nullptr);
	~Cube();

	/* Draw the cube */
	virtual void render() const;

	virtual void load(GLSLProgram* pShader);

	void renderFace(drw::face whichFace, const glm::mat4& faceMat); 
private:
	void init_vertices();
	void init_texCoords();
	void init_normals();
	void init_indices();

private:
	Texture* m_pTexture;
};



#endif