#ifndef DJW_GLSLPROGRAM_HPP
#define DJW_GLSLPROGRAM_HPP

#include <vector>
#include <string>
#include <gl_core_4_1.h>
#include <glm/glm.hpp>

namespace GLSLShader {
	enum GLSLShaderType {
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER,
		TESS_CONTROL = GL_TESS_CONTROL_SHADER,
		TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
		//COMPUTE = GL_COMPUTE_SHADER
	};
};


class GLSLProgram{
public:
	GLSLProgram(const std::string& vertex_shader_fn,
				const std::string& frag_shader_fn);

	GLSLProgram(std::vector<std::string> const& shader_filenames);
private:
	GLSLProgram(GLSLProgram const&);
	void operator=(GLSLProgram const&);

public:

	~GLSLProgram();

	void enable();
	void disable();

	bool add_shader(GLenum shader_type, const std::string& filename);
	bool finalise();

	GLuint getProgram() const {return m_shaderProg;}

	GLint getUniformLocation(const std::string& uniformName);
	GLint getProgramParameter(GLint parameter);

	void setAttribute(const std::string& attribName, GLint size,
		GLenum dataType = GL_FLOAT, GLboolean normalised = GL_FALSE,  GLint stride = 0, GLint offset = 0);

	void bindAttributeLocation(GLuint location, const std::string& name);
	void bindFragDataLocation(GLuint location, const std::string& name);

	void setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z);

	void setUniform(const std::string& name, GLfloat val);
	void setUniform(const std::string& name, GLboolean val);
	void setUniform(const std::string& name, GLint val);
	void setUniform(const std::string& name, GLuint val);

	void setUniform(const std::string& name, const glm::vec2& v);
	void setUniform(const std::string& name, const glm::vec3& v);
	void setUniform(const std::string& name, const glm::vec4& v);
	void setUniform(const std::string& name, const glm::mat3& m);
	void setUniform(const std::string& name, const glm::mat4& m);

private:
	typedef std::vector<GLuint> shaderObjects;
	shaderObjects m_shaderObjList;
	GLuint m_shaderProg;
	std::string m_shaderName;
};



#endif
