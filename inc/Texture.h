#ifndef DJW_TEXTURE_HPP
#define DJW_TEXTURE_HPP

#include <gl_core_4_2.h>
#include <string>
#include <vector>

/*
	Currently only uses a single texture target for each file passed in (typically GL_TEXTURE_2D) 
	this is fine if all the files passed in are all 2D textures (or 1D textures or 3D textures) but ...
	@TODO: make the texture targets an array or vector, the elements of which correspond to the 
	filenames passed in.
*/


class Texture{
public:
	Texture(	GLenum textureTarget, 
				const std::string& filename,
				bool meshFlippedUV = false);

	Texture(	GLenum textureTarget, 
				const std::vector<std::string>& filenames );

	~Texture();

	bool load();	

	void bind();
	void unbind();

protected:
	std::vector<std::string> m_filenames;
	GLenum m_textureTarget;
	std::vector<GLuint> m_textureObjs;
	bool m_meshFlippedUV;
};


#endif