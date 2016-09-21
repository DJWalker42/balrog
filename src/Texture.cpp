#include <Texture.h>
#include <SOIL2.h>
#include <iostream>

Texture::Texture(	GLenum texTarg,
					const std::string& fn,
					bool flipped) :
					m_filenames(1, fn),
					m_textureTarget(texTarg),
					m_textureObjs(1, 0),
					m_meshFlippedUV(flipped)
{
	//this base constructor is used in the construction of a cubeMapTexture which has its own load
	if (texTarg != GL_TEXTURE_CUBE_MAP) {
		if(!load()){
			std::cout << glGetError() << "\n";
			throw std::runtime_error("Texture failed to load from " + fn + "\n");
		}
	}
}

Texture::Texture(	GLenum texTarg,
					const std::vector<std::string>& fns ) :
					m_filenames(fns),
					m_textureTarget(texTarg),
					m_textureObjs(fns.size(), 0)
{
	if (!load()){
		std::cout << glGetError() << "\n";
		throw std::runtime_error("Texture failed to load one of the files\n");
	}
}


Texture::~Texture()
{
	glDeleteTextures(m_textureObjs.size(), m_textureObjs.data());
}

bool Texture::load()
{
	glGenTextures(m_textureObjs.size(), m_textureObjs.data());

	for(size_t i = 0; i < m_textureObjs.size(); ++i){
		m_textureObjs[i] = SOIL_load_OGL_texture
			(	m_filenames[i].c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | (m_meshFlippedUV ? 0 : SOIL_FLAG_INVERT_Y)
			);

		if (0 == m_textureObjs[i]){
			std::cout << "SOIL loading error: " << SOIL_last_result() << "\n";
			return false;
		}

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(m_textureTarget, m_textureObjs[i]);

		glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	return true; //glGetError() == GL_NO_ERROR;
}

void Texture::bind()
{
	for(size_t i = 0; i < m_textureObjs.size(); ++i){
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(m_textureTarget, m_textureObjs[i]);
	}
}

void Texture::unbind()
{
	for (size_t i = 0; i < m_textureObjs.size(); ++i){
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(m_textureTarget, 0);
	}
}
