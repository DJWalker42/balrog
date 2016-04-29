#include <Sphere.h>

#include <cstdio>
#include <cmath>

#include <glm/gtc/constants.hpp>

Sphere::Sphere(	float rad, 
				GLuint sl, 
				GLuint st,
				const glm::mat4& initialPos,
				Texture* pTexture) :
				Renderable(4, initialPos),
				m_pTexture(pTexture),
				m_radius(rad), 
				m_slices(sl), 
				m_stacks(st),
				m_nVerts((m_slices + 1) * (m_stacks + 1)),
				m_elements((m_slices * 2 * (m_stacks - 1))*3)
{
	// Verts
	float * v = new float[3 * m_nVerts];
	// Normals
	float * n = new float[3 * m_nVerts];
	// Tex coords
	float * tex = new float[2 * m_nVerts];
	// Elements
	unsigned int * el = new unsigned int[m_elements];

	// Generate the vertex data
	generateVerts(v, n, tex, el);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, (3 * m_nVerts) * sizeof(float), v, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, (3 * m_nVerts) * sizeof(float), n, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, (2 * m_nVerts) * sizeof(float), tex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elements * sizeof(unsigned int), el, GL_STATIC_DRAW);

	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tex;

	glBindVertexArray(0);
}


void Sphere::load(GLSLProgram* pShader){
	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	pShader->setAttribute("position", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	pShader->setAttribute("normal", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	pShader->setAttribute("texCoord", 2); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]); //possibly not necessary

	glBindVertexArray(0);
}


void Sphere::render() const {
	glBindVertexArray(m_vao);
	if (m_pTexture){
		m_pTexture->bind();
		glDrawElements(GL_TRIANGLES, m_elements, GL_UNSIGNED_INT, 0);
		m_pTexture->unbind();
	} else {
		glDrawElements(GL_TRIANGLES, m_elements, GL_UNSIGNED_INT, 0);
	}
}

void Sphere::generateVerts(float * verts, float * norms, float * tex,
	unsigned int * el){
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / m_slices;
	GLfloat phiFac = glm::pi<float>() / m_stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= m_slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / m_slices;
		for (GLuint j = 0; j <= m_stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / m_stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = m_radius * nx; verts[idx + 1] = m_radius * ny; verts[idx + 2] = m_radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < m_slices; i++) {
		GLuint stackStart = i * (m_stacks + 1);
		GLuint nextStackStart = (i + 1) * (m_stacks + 1);
		for (GLuint j = 0; j < m_stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			} else if (j == m_stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			} else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}