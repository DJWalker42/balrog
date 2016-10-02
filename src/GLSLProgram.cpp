#include <ShaderUtils.h>
#include <GLSLProgram.h>
#include <iostream>

#include <stdint.h> //suppress warning about void* different size to int ln 167

namespace GLSLShaderInfo {
	struct shader_file_extension {
		std::string ext;
		GLSLShader::GLSLShaderType type;
	};

	struct shader_file_extension extensions[] =
	{
		{ ".vs", GLSLShader::VERTEX },
		{ ".vert", GLSLShader::VERTEX },
		{ ".gs", GLSLShader::GEOMETRY },
		{ ".geom", GLSLShader::GEOMETRY },
		{ ".tcs", GLSLShader::TESS_CONTROL },
		{ ".tes", GLSLShader::TESS_EVALUATION },
		{ ".fs", GLSLShader::FRAGMENT },
		{ ".frag", GLSLShader::FRAGMENT },
		//{ ".cs", GLSLShader::COMPUTE }
	};
}


GLSLProgram::GLSLProgram(	const std::string& vertex_shader_fn,
							const std::string& frag_shader_fn) :
							m_shaderObjList(),
							m_shaderProg(glCreateProgram()),
							m_shaderName(GLSLUtils::getFilename(vertex_shader_fn))
{
	if (m_shaderProg == 0) {
		throw(std::runtime_error("Error creating a shader program\n"));
	}

	if(!add_shader(GL_VERTEX_SHADER, vertex_shader_fn)) {
		throw(std::runtime_error("Error attaching the vertex shader\n"));
	}

	if(!add_shader(GL_FRAGMENT_SHADER, frag_shader_fn)) {
		throw(std::runtime_error("Error attaching the fragment shader\n"));
	}

	if(!finalise()) {
		throw(std::runtime_error("Error compiling / linking the shader program"));
	}
}

GLSLProgram::GLSLProgram(	const std::vector<std::string>& shader_fns) :
							m_shaderObjList(),
							m_shaderProg(glCreateProgram()),
							m_shaderName(GLSLUtils::getFilename(shader_fns[0]))
{
	if (m_shaderProg == 0) {
		throw std::runtime_error("Error creating a shader program\n");
	}

	int numExts = sizeof(GLSLShaderInfo::extensions) / sizeof(GLSLShaderInfo::shader_file_extension);

	for(auto it = shader_fns.begin(); it != shader_fns.end(); ++it) {
		std::string ext = GLSLUtils::getExtension(*it);
		GLSLShader::GLSLShaderType type = GLSLShader::VERTEX;
		int i = 0;
		while (i < numExts && ext != GLSLShaderInfo::extensions[i].ext) {
			++i;
		}

		if ( i == numExts ) {
			//didn't find a match - throw exception
			std::string errMsg = "Unrecognised extension: " + ext;
			throw std::runtime_error(errMsg);
		}

		type = GLSLShaderInfo::extensions[i].type;

		if (!add_shader(type, *it)){
			std::string errMsg = "Error attaching shader: " + *it;
			throw std::runtime_error(errMsg);
		}

	}

	if (!finalise()) {
		throw std::runtime_error("Error compiling / linking the shader program");
	}
}


GLSLProgram::~GLSLProgram(){
	/*
		as finalise() does the shader clean-up this loop is somewhat redundant, but I like belt and braces
		when it comes to memory de-allocation.
	*/

	for(shaderObjects::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); ++it) {
		glDeleteShader(*it);
	}

	if (m_shaderProg != 0){
		glDeleteProgram(m_shaderProg);
		m_shaderProg = 0;
	}
}

bool GLSLProgram::add_shader(GLenum sType, const std::string& fn){
	GLuint shader = GLSLUtils::create_shader(fn, sType);
	if (shader == 0) {
		return false;
	} else {
		m_shaderObjList.push_back(shader); //we store the handle to delete the shader later on
		glAttachShader(m_shaderProg, shader);
		return true;
	}
}

bool GLSLProgram::finalise(){
	GLint success = 0;
	glLinkProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &success);
	if (success == 0) {
		GLSLUtils::print_log(m_shaderProg);
		return false;
	}

	glValidateProgram(m_shaderProg);
	glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &success);
	if (success == 0) {
		GLSLUtils::print_log(m_shaderProg);
		return false;
	}

	//free up memory from shader objects added to program (now compiled and linked)
	for (shaderObjects::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++) {
		glDeleteShader(*it);
	}

	m_shaderObjList.clear();

	return true;//glGetError() == GL_NO_ERROR;
}

void GLSLProgram::enable(){
	glUseProgram(m_shaderProg);
}

void GLSLProgram::disable(){
	glUseProgram(0);
}

void GLSLProgram::bindAttributeLocation(GLuint location, const std::string& name){
	glBindAttribLocation(m_shaderProg, location, name.c_str());
}

void GLSLProgram::bindFragDataLocation(GLuint location, const std::string& name){
	glBindFragDataLocation(m_shaderProg, location, name.c_str());
}

void GLSLProgram::setAttribute(const std::string& aName, GLint size,
	GLenum dType, GLboolean norm, GLint stride, GLint offset){
	GLint loc = glGetAttribLocation(m_shaderProg, aName.c_str());
	if (loc < 0){
		throw std::runtime_error("The attribute " + aName + " is not delcared in the " + m_shaderName + " program\n");
	}
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, size, dType, norm, stride, (void*)(intptr_t)offset); //suppress warning with type cast.
}

GLint GLSLProgram::getUniformLocation(const std::string& name){
	GLint location = glGetUniformLocation(m_shaderProg, name.c_str());
	if (location < 0) {
		throw std::runtime_error( "Unable to find the uniform " + name + "in the " + m_shaderName + " program\n");
	}

	return location;
}

GLint GLSLProgram::getProgramParameter(GLint param){
	GLint retval;
	glGetProgramiv(m_shaderProg, param, &retval);
	return retval;
}

void GLSLProgram::setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform3f(loc, x, y, z);
}

void GLSLProgram::setUniform(const std::string& name, GLfloat val){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform1f(loc, val);
}

void GLSLProgram::setUniform(const std::string& name, GLboolean val){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform1i(loc, val);
}

void GLSLProgram::setUniform(const std::string& name, GLint val){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform1i(loc, val);
}

void GLSLProgram::setUniform(const std::string& name, GLuint val){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform1ui(loc, val);
}

void GLSLProgram::setUniform(const std::string& name, const glm::vec2& v){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform2f(loc, v.x, v.y);
}

void GLSLProgram::setUniform(const std::string& name, const glm::vec3& v){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform3f(loc, v.x, v.y, v.z);
}

void GLSLProgram::setUniform(const std::string& name, const glm::vec4& v){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void GLSLProgram::setUniform(const std::string& name, const glm::mat3& m){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::setUniform(const std::string& name, const glm::mat4& m){
	GLint loc = glGetUniformLocation(m_shaderProg, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}
