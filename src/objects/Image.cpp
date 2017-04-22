#include "Image.h"

#include <string>
#include "glm/glm.hpp"
#include "core/Texture.h"
#include "core/Program.h"

///////////////////////////////////////////////////////////
//Shaders
///////////////////////////////////////////////////////////
//Move to external file

const char* image_vertex_shader = R"glsl(
#version 100

attribute vec4 position;

varying vec2 vertex_uv;

uniform mat4 world_matrix;
uniform mat4 model_matrix;

void main() {
	gl_Position = position;
	gl_Position = world_matrix * model_matrix * gl_Position;

	vertex_uv.x = position.x <= 0.0 ? 0.0 : 1.0;
	vertex_uv.y = position.y <= 0.0 ? 1.0 : 0.0;
}

)glsl";


const char* image_fragment_shader = R"glsl(
#version 100

varying vec2 vertex_uv;
//out vec4 out_color;

uniform sampler2D image;

void main() {
	gl_FragColor = texture2D(image, vertex_uv);
	//gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}

)glsl";

//////////////////////////////////////////////////////////
//Model
//////////////////////////////////////////////////////////

const glm::vec4 model[] = {
	glm::vec4(-1, -1, -1, 1),
	glm::vec4(-1, 1, -1, 1),
	glm::vec4(1, 1, -1, 1),
	glm::vec4(1, -1, -1, 1)
};


//////////////////////////////////////////////////////////
//Static Variables
//////////////////////////////////////////////////////////

GLuint Image::model_buffer = 0;
Program Image::program;

//////////////////////////////////////////////////////////
//Member functions
//////////////////////////////////////////////////////////

Image::Image(std::string image_file, std::string instance_name) : Object(instance_name) {
	if (image_file.length() > 0)
		setContent(image_file);

	if (program.getId() == 0) {
		glGenBuffers(1, &model_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(model), model, GL_STATIC_DRAW);
		
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

		program.load(image_vertex_shader, image_fragment_shader);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture.getId());
	}
};


void Image::setSize(glm::vec2 size) {
	if (size.x / size.y > shape)
		setScale(glm::vec3(size.y * shape, size.y, 1));
	else
		setScale(glm::vec3(size.x, size.x / shape, 1));
}


int Image::setContent(std::string image_file) {
	if (texture.loadFile(image_file)) {
		shape = texture.getWidth() / (float)texture.getHeight();
		setSize(glm::vec2(0.5));
		return 1;
	}
	else {
		return 0;
	}
}


void Image::render() {
	glUseProgram(program.getId());

	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(program.uniformLocation("model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
	glUniformMatrix4fv(program.uniformLocation("world_matrix"), 1, GL_FALSE, (float*)&world_matrix);

	glUniform1i(program.uniformLocation("image"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.getId());

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
