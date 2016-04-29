#include <Cube.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>


Cube::Cube(	const glm::mat4& model,
			Texture* pTexture) :
			Renderable(4, model),
			m_pTexture(pTexture)
{
	init_vertices();
	init_texCoords();
	init_normals();
	init_indices();

	glBindVertexArray(0);
}

Cube::~Cube()
{
	if (m_buffers[0] != 0) {
		glDeleteBuffers(m_buffers.size(), m_buffers.data());
	}

	if (m_vao != 0) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
}

void Cube::load(GLSLProgram* pShader)
{
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	pShader->setAttribute("position", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	pShader->setAttribute("texCoord", 2);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	pShader->setAttribute("normal", 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);

	glBindVertexArray(0);
}

void Cube::init_vertices()
{
	GLfloat cube_vertices[] = {
		//cube
		// front
		-.5f, -.5f, .5f,
		.5f, -.5f, .5f,
		.5f, .5f, .5f,
		-.5f, .5f, .5f,
		// top
		-.5f, .5f, .5f,
		.5f, .5f, .5f,
		.5f, .5f, -.5f,
		-.5f, .5f, -.5f,
		// back
		.5f, -.5f, -.5f,
		-.5f, -.5f, -.5f,
		-.5f, .5f, -.5f,
		.5f, .5f, -.5f,
		// bottom
		-.5f, -.5f, -.5f,
		.5f, -.5f, -.5f,
		.5f, -.5f, .5f,
		-.5f, -.5f, .5f,
		// left
		-.5f, -.5f, -.5f,
		-.5f, -.5f, .5f,
		-.5f, .5f, .5f,
		-.5f, .5f, -.5f,
		// right
		.5f, -.5f, .5f,
		.5f, -.5f, -.5f,
		.5f, .5f, -.5f,
		.5f, .5f, .5f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
}

void Cube::init_texCoords()
{
	GLfloat cube_texCoords[2 * 4 * 6] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	for (int i = 1; i < 6; ++i) {
		memcpy(&cube_texCoords[i * 2 * 4], cube_texCoords, 2 * 4 * sizeof(GLfloat));
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texCoords), cube_texCoords, GL_STATIC_DRAW);
}


void Cube::init_normals()
{
	GLfloat cube_normals[] = {

		/*Front*/
		0.f, 0.f, 1.f,
		0.f, 0.f, 1.f,
		0.f, 0.f, 1.f,
		0.f, 0.f, 1.f,

		/*Top*/ 
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 1.f, 0.f,

		/*Back*/
		0.f, 0.f, -1.f,
		0.f, 0.f, -1.f,
		0.f, 0.f, -1.f,
		0.f, 0.f, -1.f,

		/*Bottom*/
		0.f, -1.f, 0.f,
		0.f, -1.f, 0.f,
		0.f, -1.f, 0.f,
		0.f, -1.f, 0.f,

		/*Left*/
		-1.f, 0.f, 0.f,
		-1.f, 0.f, 0.f,
		-1.f, 0.f, 0.f,
		-1.f, 0.f, 0.f,

		/*Right*/
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
}

void Cube::init_indices()
{
	GLushort cube_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20,
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

}

void Cube::render() const 
{
	glBindVertexArray(m_vao);
	if (m_pTexture) {
		m_pTexture->bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);	
		m_pTexture->unbind();
	} else {
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}

	glBindVertexArray(0);
}


void Cube::renderFace(drw::face which, const glm::mat4& faceMat)
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*) (which * 6 * sizeof(GLushort)));
	glBindVertexArray(0);
}