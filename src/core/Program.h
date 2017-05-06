#pragma once

#include "gl_inc.h"
#include <string>
#include <map>

class Program {
public:
	Program() : id(0) { };

	//void load(const char*, const char*);
	int getId();
	GLint uniformLocation(std::string);

	static Program* getProgram(std::string);

private:
	static std::map<std::string, Program> program_cache;
	static GLuint loadShader(std::string, GLuint);

	GLint id;
	std::map<std::string, GLint> uniform_location;
};
