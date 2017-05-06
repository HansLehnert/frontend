#include <core/Program.h>

#include <iostream>
#include <fstream>
#include <sstream>

const char* SHADER_PATH = "./shader/";

std::map<std::string, Program> Program::program_cache;


int Program::getId() {
	return id;
}


Program* Program::getProgram(std::string name) {
	if (program_cache.find(name) == program_cache.end()) {
		//If shader hasn't been used before, compile it
		Program new_program;
		
		new_program.id = glCreateProgram();

		std::string vertex_filename = SHADER_PATH + name + "_vert.glsl";
		std::string fragment_filename = SHADER_PATH + name + "_frag.glsl";

		GLuint vertex_shader = loadShader(vertex_filename, GL_VERTEX_SHADER);
		GLuint fragment_shader = loadShader(fragment_filename, GL_FRAGMENT_SHADER);

		if (vertex_shader)
			glAttachShader(new_program.id, vertex_shader);
		else
			std::cout << "[Program]\t Failed to load " << vertex_filename << std::endl;

		if (fragment_shader)
			glAttachShader(new_program.id, fragment_shader);
		else
			std::cout << "[Program]\t Failed to load " << fragment_filename << std::endl;

		glLinkProgram(new_program.id);

		if (vertex_shader) {
			glDetachShader(new_program.id, vertex_shader);
			glDeleteShader(fragment_shader);
		}

		if (vertex_shader) {
			glDetachShader(new_program.id, fragment_shader);
			glDeleteShader(fragment_shader);
		}

		//Get uniform locations
		/*GLint uniform_count;
		glGetProgramiv(new_program.id, GL_ACTIVE_UNIFORMS, &uniform_count);
		for (int i = 0; i < uniform_count; i++) {
			Uniform new_uniform;
			GLsizei length;
			GLchar uniform_name[16];
	
			glGetActiveUniform(new_program.id, i, 16, &length, &(new_uniform.size), &(new_uniform.type), uniform_name);
			new_uniform.loc = glGetUniformLocation(new_program.id, uniform_name);

			new_program.uniform[uniform_name] = new_uniform;
		}*/

		program_cache[name] = new_program;
	}
	return &(program_cache[name]);
}


GLuint Program::loadShader(std::string path, GLuint type) {
	GLuint shader;

	std::ifstream shader_file(path);
	std::stringstream file_buffer;
	if (!shader_file.is_open()) {
		return 0;
	}

	file_buffer << shader_file.rdbuf();
	std::string file_content = file_buffer.str();
	const char* file_content_c = file_content.c_str();
	shader_file.close();

	shader = glCreateShader(type);
	glShaderSource(shader, 1, &file_content_c, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint log_length;
		GLchar* log_content;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
		log_content = new GLchar[log_length];
		glGetShaderInfoLog(shader, log_length, NULL, log_content);
		std::cout << log_content << std::endl;
		glDeleteShader(shader);
		delete log_content;
		return 0;
	}

	return shader;
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