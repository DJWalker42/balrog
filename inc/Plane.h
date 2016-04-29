#ifndef DJW_PLANE_H
#define DJW_PLANE_H

#include <Renderable.h>
#include <Texture.hpp>

class Plane : public Renderable
{
public:
	Plane(	float xSize, 
			float zSize, 
			int xDivs, 
			int zDivs, 
			Texture* pTexture = nullptr, 
			float smax = 1.0f, 
			float tmax = 1.0f);

	virtual void render() const;

	virtual void load (GLSLProgram* pShader);

private:
	void initPlane();

private:
	Texture* m_pTexture;
	int m_faces;
};

#endif // VBOPLANE_H