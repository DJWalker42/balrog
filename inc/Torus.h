#ifndef DJW_TORUS_H
#define DJW_TORUS_H

#include <Renderable.h>
#include <Texture.hpp>

class Torus : public Renderable
{
public:
	Torus(	const glm::mat4& initialPos,
			Texture* pTexture,
			float outerRadius, 
			float innerRadius, 
			int numSides, 
			int numRings );

	~Torus();

	virtual void render() const;

	virtual void load(GLSLProgram* pShader);

private:
	void generateVerts(float *, float *, float *, unsigned int *,
		float, float);

private:
	Texture* m_pTexture;
	float m_outerRadius;
	float m_innerRadius;
	int m_rings;
	int m_sides;
	int m_faces;
};

#endif