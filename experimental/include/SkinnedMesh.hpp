#ifndef DJW_SKINNEDMESH_HPP
#define DJW_SKINNEDMESH_HPP

#include <BasicMesh.hpp>
#include <assimp/scene.h>

class SkinnedMesh : public BasicMesh
{
public:
	SkinnedMesh();
	~SkinnedMesh();

	size_t getNumBones() const {return m_numBones;}

	void boneTransform(float t_in_seconds, std::vector<glm::mat4>& transforms);

	static const unsigned int NUM_BONES_PER_VERTEX = 4;

private: 
	struct boneInfo{
		boneInfo() : s_offset(0.f), s_finalTrans(0.f)
		{}

		glm::mat4 s_offset;
		glm::mat4 s_finalTrans;
	}; 

	struct vertexBoneData
	{
		vertexBoneData() : s_ID(), s_weights()
		{
			reset(); //ensure we get zeros on construction.
		}

		void reset()
		{
			s_ID.fill(0);
			s_weights.fill(0.f);
		}

		void addBoneData(size_t boneID, float weight);

		std::array<size_t, NUM_BONES_PER_VERTEX> s_ID;
		std::array<float, NUM_BONES_PER_VERTEX> s_weights;
	};

	void interpScaling (aiVector3D& out, float animTime, const aiNodeAnim* pNodeAnim);
	void interpRotation(aiVector3D& out, float animTime, const aiNodeAnim* pNodeAnim);
	void interpPosition(aiVector3D& out, float animTime, const aiNodeAnim* pNodeAnim);

	size_t findScaling (float animTime, const aiNodeAnim* pNodeAnim);
	size_t findRotation(float animTime, const aiNodeAnim* pNodeAnim);
	size_t findPosition(float animTime, const aiNodeAnim* pNodeAnim);

	const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string& nodeName); 

	void readNodeHeirarchy(float animTime, const aiNode* pNode, const glm::mat4& parentTrans);

	void loadBones(size_t meshIndex, const aiMesh* pMesh, std::vector<vertexBoneData>& bones);

protected:


private:
	std::map<std::string, size_t> m_BoneMapping; // maps a bone name to its index
	size_t m_numBones;
	std::vector<boneInfo> m_boneInfo;
	glm::mat4 m_GlobalInverseTransform;
	const aiScene* m_pScene;
};



#endif