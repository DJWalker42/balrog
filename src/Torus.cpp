#include <Torus.h>
#include <glm/gtc/constants.hpp>


Torus::Torus(	const glm::mat4& initialPos,
				Texture* pTexture,
				float outerRadius,
				float innerRadius,
				int nsides,
				int nrings	) :
				Renderable(4, initialPos),
				m_pTexture(pTexture),
				m_outerRadius(outerRadius),
				m_innerRadius(innerRadius),
				m_rings(nrings),
				m_sides(nsides),				
				m_faces( m_rings * m_sides )
{
	int nVerts = m_sides * (m_rings + 1);   // One extra ring to duplicate first ring

	// Verts
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];
	// Tex coords
	float * tex = new float[2 * nVerts];
	// Elements
	unsigned int * el = new unsigned int[6 * m_faces];

	// Generate the vertex data
	generateVerts(v, n, tex, el, m_outerRadius, m_innerRadius);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), v, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, (3 * nVerts) * sizeof(float), n, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, (2 * nVerts) * sizeof(float), tex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * m_faces * sizeof(unsigned int), el, GL_STATIC_DRAW);

	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tex;

	glBindVertexArray(0);
}

Torus::~Torus()
{}


void Torus::load(GLSLProgram* pShader)
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	pShader->setAttribute("position", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	pShader->setAttribute("normal", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	pShader->setAttribute("texCoord", 2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);
	glBindVertexArray(0);
}


void Torus::render() const
{
	glBindVertexArray(m_vao);

	m_pTexture->bind();
	glDrawElements(GL_TRIANGLES, 6 * m_faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
	m_pTexture->unbind();

	glBindVertexArray(0);
}

void Torus::generateVerts(float * verts, float * norms, float * tex,
	unsigned int * el,
	float outerRadius, float innerRadius)
{
	float ringFactor = glm::two_pi<float>() / m_rings;
	float sideFactor = glm::two_pi<float>() / m_sides;
	int idx = 0, tidx = 0;
	for (int ring = 0; ring <= m_rings; ring++) {
		float u = ring * ringFactor;
		float cu = cos(u);
		float su = sin(u);
		for (int side = 0; side < m_sides; side++) {
			float v = side * sideFactor;
			float cv = cos(v);
			float sv = sin(v);
			float r = (outerRadius + innerRadius * cv);
			verts[idx] = r * cu;
			verts[idx + 1] = r * su;
			verts[idx + 2] = innerRadius * sv;
			norms[idx] = cv * cu * r;
			norms[idx + 1] = cv * su * r;
			norms[idx + 2] = sv * r;
			tex[tidx] = u / glm::two_pi<float>();
			tex[tidx + 1] = v / glm::two_pi<float>();
			tidx += 2;
			// Normalize
			float len = sqrt(norms[idx] * norms[idx] +
				norms[idx + 1] * norms[idx + 1] +
				norms[idx + 2] * norms[idx + 2]);
			norms[idx] /= len;
			norms[idx + 1] /= len;
			norms[idx + 2] /= len;
			idx += 3;
		}
	}

	idx = 0;
	for (int ring = 0; ring < m_rings; ring++) {
		int ringStart = ring * m_sides;
		int nextRingStart = (ring + 1) * m_sides;
		for (int side = 0; side < m_sides; side++) {
			int nextSide = (side + 1) % m_sides;
			// The quad
			el[idx] = (ringStart + side);
			el[idx + 1] = (nextRingStart + side);
			el[idx + 2] = (nextRingStart + nextSide);
			el[idx + 3] = ringStart + side;
			el[idx + 4] = nextRingStart + nextSide;
			el[idx + 5] = (ringStart + nextSide);
			idx += 6;
		}
	}
}
