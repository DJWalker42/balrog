#ifndef DJW_SPHERE_H
#define DJW_SPHERE_H

#include <Renderable.h>
#include <Texture.hpp>

class Sphere : public Renderable
{
public:
	Sphere(	float radius, 
			GLuint slices, 
			GLuint stacks,
			const glm::mat4& initialPos = glm::mat4(),
			Texture* pTexture = nullptr);

	virtual void render() const;
	virtual void load(GLSLProgram* pShader);

private:

	void generateVerts(float *, float *, float *, GLuint *);

private:
	Texture* m_pTexture;
	
	float m_radius;
	GLuint m_slices, m_stacks;
	GLuint m_nVerts, m_elements;
};

#endif
