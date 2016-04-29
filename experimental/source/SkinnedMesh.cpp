#include <SkinnedMesh.hpp>


void SkinnedMesh::vertexBoneData::addBoneData(size_t BoneID, float Weight)
{
	for (size_t i = 0; i < s_ID.size(); i++) {
		if (s_weights[i] == 0.0) {
			s_ID[i] = BoneID;
			s_weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	assert(0);
}
