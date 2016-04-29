#include <Plane.h>

#include <cstdio>
#include <cmath>

Plane::Plane(	float xSize, 
				float zSize, 
				int xDivs, 
				int zDivs, 
				Texture* pTexture,
				float smax, 
				float tmax) :
				Renderable(4),
				m_pTexture(pTexture),
				m_faces(xDivs * zDivs)
{
	float * v = new float[3 * (xDivs + 1) * (zDivs + 1)];
	float * n = new float[3 * (xDivs + 1) * (zDivs + 1)];
	float * tex = new float[2 * (xDivs + 1) * (zDivs + 1)];
	unsigned int * el = new unsigned int[6 * xDivs * zDivs];

	float x2 = xSize / 2.0f;
	float z2 = zSize / 2.0f;
	float iFactor = (float)zSize / zDivs;
	float jFactor = (float)xSize / xDivs;
	float texi = smax / zDivs;
	float texj = tmax / xDivs;
	float x, z;
	int vidx = 0, tidx = 0;
	for (int i = 0; i <= zDivs; i++) {
		z = iFactor * i - z2;
		for (int j = 0; j <= xDivs; j++) {
			x = jFactor * j - x2;
			v[vidx] = x;
			v[vidx + 1] = 0.0f;
			v[vidx + 2] = z;
			n[vidx] = 0.0f;
			n[vidx + 1] = 1.0f;
			n[vidx + 2] = 0.0f;
			vidx += 3;
			tex[tidx] = j * texi;
			tex[tidx + 1] = i * texj;
			tidx += 2;
		}
	}

	unsigned int rowStart, nextRowStart;
	int idx = 0;
	for (int i = 0; i < zDivs; i++) {
		rowStart = i * (xDivs + 1);
		nextRowStart = (i + 1) * (xDivs + 1);
		for (int j = 0; j < xDivs; j++) {
			el[idx] = rowStart + j;
			el[idx + 1] = nextRowStart + j;
			el[idx + 2] = nextRowStart + j + 1;
			el[idx + 3] = rowStart + j;
			el[idx + 4] = nextRowStart + j + 1;
			el[idx + 5] = rowStart + j + 1;
			idx += 6;
		}
	}

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, 3 * (xDivs + 1) * (zDivs + 1) * sizeof(float), v, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, 3 * (xDivs + 1) * (zDivs + 1) * sizeof(float), n, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	glBufferData(GL_ARRAY_BUFFER, 2 * (xDivs + 1) * (zDivs + 1) * sizeof(float), tex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * xDivs * zDivs * sizeof(unsigned int), el, GL_STATIC_DRAW);

	glBindVertexArray(0);

	delete[] v;
	delete[] n;
	delete[] tex;
	delete[] el;
}

void Plane::load(GLSLProgram* pShader)
{	
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	pShader->setAttribute("position", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	pShader->setAttribute("normal", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	pShader->setAttribute("texCoord", 2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[VB::INDEX]);

	glBindVertexArray(0);
}


void Plane::render() const {
	glBindVertexArray(m_vao);
	if(m_pTexture){
		m_pTexture->bind();
		glDrawElements(GL_TRIANGLES, 6 * m_faces, GL_UNSIGNED_INT, 0);
		m_pTexture->unbind();
	} else {
		glDrawElements(GL_TRIANGLES, 6 * m_faces, GL_UNSIGNED_INT, 0);
	}
	glBindVertexArray(0);
}
