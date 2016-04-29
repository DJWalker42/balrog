#ifndef CUBEMAPTEXTURE_HPP
#define CUBEMAPTEXTURE_HPP

#include <Texture.hpp>

class CubeMap : public Texture
{
public:
	CubeMap(	const std::string& Directory,
				const std::string& PosXFile,
				const std::string& NegXFile,
				const std::string& PosYFile,
				const std::string& NegYFile,
				const std::string& PosZFile,
				const std::string& NegZFile ); 

	/* Filename order needs to be posx, negx, posy, negy, posz, negz */
	CubeMap(	const std::string& Directory,
				const std::vector<std::string>& filenames ); 

	virtual bool load();

protected:

private:
	std::string m_cubeFileNames[6];
};



#endif