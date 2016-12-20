#include "Image.h"

#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "core/Texture.h"
#include "core/Program.h"

const char* image_vertex_shader = R"glsl(
#version 330

layout(location = 0) in vec4 position;

out vec2 vertex_uv;

uniform mat4 world_matrix;
uniform mat4 model_matrix;

void main() {
	gl_Position = position;
	gl_Position = world_matrix * model_matrix * gl_Position;
	gl_Position.z = -1;

	vertex_uv.x = position.x <= 0 ? 0 : 1;
	vertex_uv.y = position.y <= 0 ? 1 : 0;
}

)glsl";

const char* image_fragment_shader = R"glsl(
#version 330

in vec2 vertex_uv;
out vec4 out_color;

uniform sampler2D image;

void main() {
	out_color = texture(image, vertex_uv);
}

)glsl";

const glm::vec4 model[] = {
	glm::vec4(-1, -1, -1, 1),
	glm::vec4(-1, 1, -1, 1),
	glm::vec4(1, 1, -1, 1),
	glm::vec4(1, -1, -1, 1)
};

GLuint Image::model_buffer = 0;
Program Image::program;

Image::Image(std::string image_file) {
	if (image_file.length() > 0)
		setContent(image_file);

	if (program.getId() == 0) {
		glGenBuffers(1, &model_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(model), model, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

		program.load(image_vertex_shader, image_fragment_shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.getId());
	}
};

void Image::setPosition(glm::vec2 position) {
	model_matrix[3][0] = position.x;
	model_matrix[3][1] = position.y;
}

void Image::setScale(glm::vec2 scale) {
	model_matrix[0][0] = scale.x;
	model_matrix[1][1] = scale.y;
}

void Image::setSize(glm::vec2 size) {
	if (size.x / size.y > shape)
		setScale(glm::vec2(size.y * shape, size.y));
	else
		setScale(glm::vec2(size.x, size.x / shape));
}

int Image::setContent(std::string image_file) {
	if (texture.load(image_file.c_str())) {
		shape = texture.getWidth() / (float)texture.getHeight();
		setSize(glm::vec2(0.5));
		return 1;
	}
	else {
		return 0;
	}
}

void Image::render() {
	if (!visible)
		return;

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