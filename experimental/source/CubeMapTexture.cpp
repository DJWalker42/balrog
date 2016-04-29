#include <CubeMapTexture.hpp>
#include <SOIL2.h>

static const GLenum types[6] = {	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
									GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
									GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
									GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
									GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
									GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 
};

CubeMap::CubeMap(	const std::string& Directory,
					const std::string& PosXFile,
					const std::string& NegXFile,
					const std::string& PosYFile,
					const std::string& NegYFile,
					const std::string& PosZFile,
					const std::string& NegZFile ) :
					Texture(GL_TEXTURE_CUBE_MAP, Directory) //this sets m_textureTarget == GL_TEXTURE_CUBE_MAP 
{
	std::string::const_iterator cit = m_filenames[0].end();
	--cit;
	std::string root = (*cit == '/') ? m_filenames[0] : m_filenames[0] + "/";

	m_cubeFileNames[0] = root + PosXFile;
	m_cubeFileNames[1] = root + NegXFile;
	m_cubeFileNames[2] = root + PosYFile;
	m_cubeFileNames[3] = root + NegYFile;
	m_cubeFileNames[4] = root + PosZFile;
	m_cubeFileNames[5] = root + NegZFile;

	load();
}

CubeMap::CubeMap(	const std::string& Directory,
					const std::vector<std::string> & filenames) :
					Texture(GL_TEXTURE_CUBE_MAP, Directory)
{
	std::string::const_iterator cit = m_filenames[0].end();
	--cit;
	std::string root = (*cit == '/') ? m_filenames[0] : m_filenames[0] + "/";

	for (int i = 0; i < 6; ++i) {
		m_cubeFileNames[i] = root + filenames[i];
	}
	
	load();
}


bool CubeMap::load()
{
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &m_textureObjs[0]);
	glBindTexture(m_textureTarget, m_textureObjs[0]);

	int width, height;
	unsigned char* image;
	for (int i = 0; i < 6; ++i) {
		image = SOIL_load_image(m_cubeFileNames[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if(image == NULL) return false;
		glTexImage2D(types[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}