#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <gl_core_4_1.h>
#include <glm/glm.hpp>
#include <Renderable.h>
#include <GLSLProgram.h>

#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>

#include <iostream>

#define VBO_NUM 3

class pointCloud : public Renderable{
public:

	pointCloud(	const std::string& points_file,
				const std::string& colour_file = std::string(),
				const std::string& normal_file = std::string(),
				GLfloat pointSize = 1.f);

	~pointCloud();

	bool readPointCloud( const std::string& filename );

	bool readImage( const std::string& filename );

	void readNormals( const std::string& filename );

	virtual void load(GLSLProgram*);
	virtual void render() const;

protected:

private: /* Un-copyable */
	pointCloud( const pointCloud& );
	pointCloud& operator=(const pointCloud&);

private:

	/* Shifts the entire point cloud such that it's centroid is at the origin of the projection space*/
	void shift()
	{
		m_numFloats = m_vVertices.size();

		float centroid_x = 3 * m_sum_x / m_numFloats;
		float centroid_y = 3 * m_sum_y / m_numFloats;
		float centroid_z = 3 * m_sum_z / m_numFloats;

		for (int i = 0; i < m_numFloats; i += 3)
		{
			m_vVertices[i    ] -= centroid_x;
			m_vVertices[i + 1] -= centroid_y;
			m_vVertices[i + 2] -= centroid_z;

		}
	}

	bool readPoint(const std::string& line)
	{
		std::istringstream is(line);
		float x, y, z;

		//read in values
		is >> x;
		is >> y;
		is >> z;

		if( fabs(z) < 9000.f ) //if z == -10000 or 10000 we want to skip it as its a missing value
		{
			m_max_x = fabs(x) > m_max_x ? fabs(x) : m_max_x;
			m_max_y = fabs(y) > m_max_y ? fabs(y) : m_max_y;
			m_max_z = fabs(z) > m_max_z ? fabs(z) : m_max_z;

			m_min_z = fabs(z) < m_min_z ? fabs(z) : m_min_z;

			m_sum_x += x;
			m_sum_y += y;
			m_sum_z += z;

			m_vVertices.push_back(x);
			m_vVertices.push_back(y);
			m_vVertices.push_back(z);

			return true;
		}
		else
			return false;

	}

	void readNormalPoint(const std::string& line)
	{
		std::istringstream is(line);
		float x, y, z;

		//read in values
		is >> x;
		is >> y;
		is >> z;

		m_vNormals.push_back(x);
		m_vNormals.push_back(y);
		m_vNormals.push_back(z);
	}
private:
	std::vector<GLfloat> m_vVertices;
	std::vector<GLfloat> m_vNormals;
	std::vector<GLubyte> m_vColour;

	GLfloat m_pointSize;

	bool m_normals;

	GLsizei m_numFloats;

	float m_max_x;
	float m_max_y;
	float m_max_z;
	float m_min_z;

	float m_sum_x;
	float m_sum_y;
	float m_sum_z;

	std::list<int> m_listSkip;
};

#endif
