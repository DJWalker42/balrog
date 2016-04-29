#ifndef SHADER_UTILS_H
#define SHADER_UTILS_H

#define _CRT_SECURE_NO_WARNINGS

#include <gl_core_4_2.h>
#include <string>

namespace GLSLUtils{
	std::string read_file(const std::string& filename);
	GLuint create_shader(const std::string& filename, GLenum type );
	void print_log(GLuint object);
	std::string getFilename(const std::string& str);
	std::string getExtension(const std::string& name);
}

#endif