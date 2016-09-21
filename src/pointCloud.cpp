#include <pointCloud.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>


pointCloud::pointCloud(	const std::string& points_file,
						const std::string& colour_file,
						const std::string& normal_file,
						GLfloat pointSize):
						Renderable(3),
						m_vVertices(),
						m_vNormals(),
						m_vColour(),					
						m_pointSize(pointSize),
						m_normals(normal_file.empty()? false : true),
						m_numFloats(0),
						m_max_x(0.f),
						m_max_y(0.f),
						m_max_z(0.f),
						m_min_z(10000.f),
						m_sum_x(0.f),
						m_sum_y(0.f),
						m_sum_z(0.f),
						m_listSkip()
{
	std::string error_msg = "pointCloud error: ";
	//populate the m_vVerticies and the m_vColour vectors
	if(!readPointCloud(points_file)) throw std::runtime_error(error_msg + "failed to read point cloud file");
	if (!readImage(colour_file)) throw std::runtime_error(error_msg + "failed to read colour image file");
	readNormals(normal_file);

	glPointSize(m_pointSize);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numFloats, m_vVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]); //here we use to the texcoord buffer to describe a colour for a point (vertex)
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLubyte) * m_numFloats, m_vColour.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_numFloats, m_vNormals.data(), GL_STATIC_DRAW);
}


pointCloud::~pointCloud()
{}


void pointCloud::load(GLSLProgram* pShader)
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::POSITION]);
	pShader->setAttribute("position", 3);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::TEXCOORD]);
	pShader->setAttribute("colour", 3, GL_UNSIGNED_BYTE, GL_TRUE);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VB::NORMAL]);
	pShader->setAttribute("normal", 3);

	glBindVertexArray(0);
}


void pointCloud::render() const
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_numFloats / 3);
	glBindVertexArray(0);
}


bool pointCloud::readPointCloud(const std::string& fn)
{
	std::ifstream file(fn);
	if(file.is_open()) {
		std::string line;
		int linearIndex = 0;

		while(std::getline(file, line))	{
			if(!readPoint(line))
				m_listSkip.push_back(linearIndex);
			linearIndex++;
		}

		file.close();
		shift(); //this assigns m_numFloats
		return true;
	} else {
		std::cout << "Unable to open " << fn << "\n";
		return false;
	}
}


bool pointCloud::readImage(const std::string& filename)
{
	if(filename.empty()){
		m_vColour.reserve(m_numFloats);
		for(int i = 0; i < m_numFloats; ++i)
			m_vColour.push_back(100); //set grey default colour
		return true;
	} else {
		//todo: read file for colour data
		std::cout << "readImage(" << filename << ") not yet implemented. Grey set by default." << std::endl;
		m_vColour.reserve(m_numFloats);
		for (int i = 0; i < m_numFloats; ++i)
			m_vColour.push_back(100); //set grey default colour
		return true;
	}
}


void pointCloud::readNormals(const std::string& fn)
{
	if (fn.empty()) {
		for (int i = 0; i < m_numFloats / 3; ++i) {
			m_vNormals.push_back(0.f);
			m_vNormals.push_back(0.f);
			m_vNormals.push_back(-1.f);
		}
		return;
	}

	std::ifstream file(fn);
	if (file.is_open()){
		std::string line;

		while (std::getline(file, line))
			readNormalPoint(line);

		file.close();
		if(m_vNormals.size() != m_numFloats)
			std::cout << "Warning: Number of normals does not match the number of points. \n\n";
	} else {
		std::cout << "Unable to open " << fn << "\n" << "Setting all normals to face viewport (0,0,-1)\n\n";
		for(int i = 0; i < m_numFloats / 3; ++i){
			m_vNormals.push_back(0.f);
			m_vNormals.push_back(0.f);
			m_vNormals.push_back(-1.f);
		}
	}
}
