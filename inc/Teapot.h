#ifndef DJW_TEAPOT_H
#define DJW_TEAPOT_H

#include <Renderable.h>
#include <Texture.h>
#include <glm/glm.hpp>

class Teapot : public Renderable{
public:
	Teapot(	int grid,
			const glm::mat4& startModel = glm::mat4(),
			const glm::mat4& lidTransform = glm::mat4(),
			Texture* pTexture = nullptr);

	~Teapot();

	virtual void load(GLSLProgram* pShader);

	virtual void render() const;
private:

	void generatePatches(float * v, float * n, float *tc, unsigned int* el, int grid);

	void buildPatchReflect(	int patchNum,
							float *B, float *dB,
							float *v, float *n, float *, unsigned int *el,
							int &index, int &elIndex, int &, int grid,
							bool reflectX, bool reflectY);

	void buildPatch(	glm::vec3 patch[][4],
						float *B, float *dB,
						float *v, float *n, float *, unsigned int *el,
						int &index, int &elIndex, int &, int grid, glm::mat3 reflect, bool invertNormal);

	void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);

	void computeBasisFunctions(float * B, float * dB, int grid);

	glm::vec3 evaluate(int gridU, int gridV, float *B, glm::vec3 patch[][4]);

	glm::vec3 evaluateNormal(int gridU, int gridV, float *B, float *dB, glm::vec3 patch[][4]);

	void moveLid(int, float *, const glm::mat4 &);

	glm::mat4 rotate(const glm::vec3& rot_vec) const;

private:
	Texture* m_pTexture;

	unsigned int m_faces;
	int m_grid;
	glm::mat4 m_lidTransform;
};

#endif // VBOTEAPOT_H
