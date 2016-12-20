#include <core/Program.h>

#include <iostream>
#include <GL/glew.h>

void Program::load(const char* vertex, const char* fragment) {
	GLuint vertex_shader, fragment_shader;
	GLint compile_success;
	
	id = glCreateProgram();
	
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compile_success);
	if (compile_success == GL_FALSE) {
		GLint log_length;
		GLchar* log_content;

		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
		log_content = new GLchar[log_length];
		glGetShaderInfoLog(vertex_shader, log_length, NULL, log_content);
		std::cout << log_content << std::endl;
		glDeleteShader(vertex_shader);
		delete(log_content);
	}
	else {
		glAttachShader(id, vertex_shader);
	}

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compile_success);
	if (compile_success == GL_FALSE) {
		GLint log_length;
		GLchar* log_content;

		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
		log_content = new GLchar[log_length];
		glGetShaderInfoLog(fragment_shader, log_length, NULL, log_content);
		std::cout << log_content << std::endl;
		glDeleteShader(fragment_shader);
		delete(log_content);
	}
	else {
		glAttachShader(id, fragment_shader);
	}

	glLinkProgram(id);
	glDetachShader(id, vertex_shader);
	glDeleteShader(vertex_shader);
	glDetachShader(id, fragment_shader);
	glDeleteShader(fragment_shader);
}

int Program::getId() {
	return id;
}

GLint Program::uniformLocation(std::string uniform) {
	std::map<std::string, GLint>::iterator uniform_search = uniform_location.find(uniform);
	if (uniform_search != uniform_location.end()) {
		return uniform_search->second;
	}
	else {
		GLint loc = glGetUniformLocation(id, uniform.c_str());
		if (loc != -1) {
			uniform_location[uniform] = loc;
			return loc;
		}
		else {
			return -1;
		}
	}
}