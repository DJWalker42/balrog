#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <GLSLProgram.hpp>
#include <glm/glm.hpp>

class Renderable{
protected:
	Renderable(size_t numBuffers, const glm::mat4& model = glm::mat4()) : 
		m_model(model),
		m_animation(),
		m_vao(),
		m_buffers(numBuffers)
	{
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(numBuffers, m_buffers.data()); 	
	}

public:
	virtual ~Renderable(){
		if (m_buffers[0] != 0) {
			glDeleteBuffers(m_buffers.size(), m_buffers.data());
		}

		if (m_vao != 0) {
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}	
	}

public:
	virtual void load(GLSLProgram* pShader) = 0;
	virtual void render() const =0;

	glm::mat4 getModelMatrix() const { return m_model; }
	glm::mat4 getModelAnimation() const { return m_model * m_animation; }

	void setModelMatrix(const glm::mat4& model) { m_model = model; }
	void setAnimation(const glm::mat4& animation) { m_animation = animation; }
	
protected:
	glm::mat4 m_model;
	glm::mat4 m_animation;
	GLuint m_vao;
	std::vector<GLuint> m_buffers; 
	enum VB{ POSITION, TEXCOORD, NORMAL, INDEX, MVP, WORLD };
};


#endif 