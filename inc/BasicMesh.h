#ifndef DJW_BASICMESH_HPP
#define DJW_BASICMESH_HPP

#include <map>
#include <vector>
#include <array>
#include <string>

#include <gl_core_4_1.h>
#include <glm/glm.hpp>


#include <ShaderUtils.h>
#include <Texture.h>
#include <Renderable.h>

struct aiScene;
struct aiMesh;

class Vertex{
public:
	Vertex() : m_position(), m_texture(), m_normal()
	{}

	Vertex(	const glm::vec3& pos,
			const glm::vec2& tex,
			const glm::vec3& norm) :
			m_position(pos),
			m_texture(tex),
			m_normal(norm)
	{}

protected:

private:
	glm::vec3 m_position;
	glm::vec2 m_texture;
	glm::vec3 m_normal;
};


class BasicMesh : public Renderable {
public:
	BasicMesh(	const glm::mat4& model,
				const std::string& file2load,
				const std::string& texFile = std::string());
	~BasicMesh();

	virtual void load(GLSLProgram* pShader);

	virtual void render() const;

	void render(size_t numInstances, const glm::mat4& MVP, const glm::mat4& worldMat);

	static const unsigned int NUM_BUFFERS = 6;

protected:

private:
	bool initFromScene(const aiScene* pScene, const std::string& filename);
	bool initMaterials(const aiScene* pScene, const std::string& filename);
	void initMesh(	const aiMesh* pMesh,
					std::vector<glm::vec3>& positions,
					std::vector<glm::vec2>& textures,
					std::vector<glm::vec3>& normals,
					std::vector<unsigned int>& indices);

	void clear();

private:

	std::string m_meshFile;

	struct MeshEntry{
		MeshEntry() :
			s_numIndices(0),
			s_baseVertex(0),
			s_baseIndex(0),
			s_materialsIndex(-1)
		{}

		int s_numIndices;
		int s_baseVertex;
		int s_baseIndex;
		int s_materialsIndex;
	};

	std::vector<MeshEntry> m_entries;
	std::vector<Texture*> m_pTextures;
};

#endif
