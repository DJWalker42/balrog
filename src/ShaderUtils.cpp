#include <ShaderUtils.h>
#include <iostream>
#include <fstream>

namespace GLSLUtils{
	std::string read_file(const std::string& filename)
	{
		std::string content;
		std::ifstream content_stream(filename, std::ios::in);
		if(content_stream.is_open()){
			std::string line;
			while(getline(content_stream, line))
				content += "\n" + line;
			content_stream.close();
		} else {
			return std::string();
		}

		return content;
	}

	GLuint create_shader(const std::string& filename, GLenum type)
	{
		std::string source = read_file(filename);
		if(source.empty()){
			std::cerr << "Error opening " << filename << "\n";
			return 0;
		}
		GLuint res = glCreateShader(type);
		const char * source_ptr = source.c_str();
		glShaderSource(res, 1, &source_ptr, NULL);
		glCompileShader(res);
		GLint compile_ok = GL_FALSE;
		glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);

		if (compile_ok == GL_FALSE) {
			std::cerr << filename << "\n";
			print_log(res);
			glDeleteShader(res);
			return 0;
		}

		return res;
	}

	/**
	* Display compilation errors from the OpenGL shader compiler
	*/
	void print_log(GLuint object)
	{
		GLint log_length = 0;
		if (glIsShader(object)){
			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
		} else if (glIsProgram(object)){
			glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
		} else {
			std::cerr << "printlog: Not a shader or a program\n";
			return;
		}

		char* log = (char*)malloc(log_length);

		if (glIsShader(object)) {
			glGetShaderInfoLog(object, log_length, NULL, log);
		} else if (glIsProgram(object)){
			glGetProgramInfoLog(object, log_length, NULL, log);
		}

		std::string slog(log); 
		std::cerr << slog << "\n";	
		free(log);
	}

	std::string getFilename(const std::string& str)
	{
		size_t location = str.find_last_of("/\\");
		std::string tmp;
		if (std::string::npos == location) {//no slashes in string
			tmp = str;
		} else {
			tmp =  str.substr(location + 1);
		}
		location = tmp.find_last_of('.');
		if (location == std::string::npos) {
			return tmp;
		} else {
			return tmp.substr(0, location);
		}
	}

	std::string getExtension(const std::string& name) 
	{
		size_t loc = name.find_last_of('.');
		if (loc != std::string::npos) {
			return name.substr(loc, std::string::npos);
		}
		return "";
	}
}