#ifndef DJW_SKYBOX_H
#define DJW_SKYBOX_H

#include <Renderable.h>
#include <CubeMapTexture.hpp>

class Skybox : public Renderable{
public:
	Skybox(CubeMap* pTexture);

	~Skybox();

	virtual void load(GLSLProgram* pShader);

	virtual void render() const;

private:
	CubeMap* m_pTexture;
};




#endif