#include <BasicMesh.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

BasicMesh::BasicMesh(	const glm::mat4& model,
						const std::string& meshFile,
						const std::string& texFile) :
						Renderable(6, model),
						m_meshFile(meshFile),
						m_entries(),
						m_pTextures()
{
	if (!texFile.empty())
		m_pTextures.push_back(new Texture(GL_TEXTURE_2D, texFile));

	Assimp::Importer assImport; //ha!!
	const aiScene* pScene = assImport.ReadFile(m_meshFile.c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_FindDegenerates);

	if (!pScene || !initFromScene(pScene, m_meshFile)) {
		clear();
		throw std::runtime_error("Exception reading " + m_meshFile + " due to: " + assImport.GetErrorString());
	}
	glBindVertexArray(0);
}

BasicMesh::~BasicMesh()
{
	clear();
}

void BasicMesh::clear()
{
	for (size_t i = 0; i < m_pTextures.size(); ++i){
		if (m_pTextures[i]) {
			delete m_pTextures[i];
			m_pTextures[i] = nullptr;
		}
	}
}

void BasicMesh::load(GLSLProgram* pShader)
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	pShader->setAttribute("position", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	pShader->setAttribute("texCoord", 2);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	pShader->setAttribute("normal", 3);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);
	glBindVertexArray(0);
}

bool BasicMesh::initFromScene(const aiScene* pScene, const std::string& filename)
{
	m_entries.resize(pScene->mNumMeshes);
	m_pTextures.resize(pScene->mNumMaterials);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<size_t> indices;

	size_t numVertices = 0;
	size_t numIndices = 0;

	//count the total number of vertices and indices
	for(size_t i = 0; i < m_entries.size(); ++i){
		m_entries[i].s_materialsIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_entries[i].s_numIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_entries[i].s_baseVertex = numVertices;
		m_entries[i].s_baseIndex = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices += m_entries[i].s_numIndices;
	}

	positions.reserve(numVertices);
	normals.reserve(numVertices);
	texcoords.reserve(numVertices);
	indices.reserve(numIndices);

	// Initialize the meshes in the scene one by one
	for (size_t i = 0; i < m_entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initMesh(paiMesh, positions, texcoords, normals, indices);
	}

	if (!initMaterials(pScene, filename)) {
		return false;
	}

	// Generate and populate the buffers with vertex attributes and the indices
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords[0]) * texcoords.size(), &texcoords[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	return glGetError() == GL_NO_ERROR;
}


void BasicMesh::initMesh(	const aiMesh* paiMesh,
							std::vector<glm::vec3>& Positions,
							std::vector<glm::vec2>& TexCoords,
							std::vector<glm::vec3>& Normals,
							std::vector<size_t>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {

		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
	}

	// Populate the index buffer
	for (size_t i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}


bool BasicMesh::initMaterials(const aiScene* pScene, const std::string& filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_pTextures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				m_pTextures[i] = new Texture(GL_TEXTURE_2D, FullPath, true); //texture target, filename, mesh has flipped UV already
			}
		}
	}

	return Ret;
}

void BasicMesh::render() const
{
	glBindVertexArray(m_vao);

	for (unsigned int i = 0; i < m_entries.size(); i++) {
		const unsigned int MaterialIndex = m_entries[i].s_materialsIndex;

		assert(MaterialIndex < m_pTextures.size());

		if (m_pTextures[MaterialIndex]) {
			m_pTextures[MaterialIndex]->bind();
		}

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_entries[i].s_numIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_entries[i].s_baseIndex),
			m_entries[i].s_baseVertex);
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}

void BasicMesh::render(size_t NumInstances, const glm::mat4& MVPMats, const glm::mat4& WorldMats)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::MVP]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, glm::value_ptr(MVPMats), GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::WORLD]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * NumInstances, glm::value_ptr(WorldMats), GL_DYNAMIC_DRAW);

	glBindVertexArray(m_vao);

	for (unsigned int i = 0; i < m_entries.size(); i++) {
		const unsigned int MaterialIndex = m_entries[i].s_materialsIndex;

		assert(MaterialIndex < m_pTextures.size());

		if (m_pTextures[MaterialIndex]) {
			m_pTextures[MaterialIndex]->bind();
		}

		glDrawElementsInstancedBaseVertex(	GL_TRIANGLES,
											m_entries[i].s_numIndices,
											GL_UNSIGNED_INT,
											(void*)(sizeof(unsigned int) * m_entries[i].s_baseIndex),
											NumInstances,
											m_entries[i].s_baseVertex);
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);
}
