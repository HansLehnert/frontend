#pragma once

#include <GL/glew.h>
#include <map>

class Program {
public:
	Program() : id(0) { };

	void load(const char*, const char*);
	int getId();
	GLint uniformLocation(std::string);
private:
	GLint id;
	std::map<std::string, GLint> uniform_location;
};