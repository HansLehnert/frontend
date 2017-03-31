#include "Text.h"

#include <string>
#include "glm/glm.hpp"
#include "core/Texture.h"
#include "core/Program.h"

const char* text_vertex_shader = R"glsl(
#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in int index;

out vec2 vertex_uv;

uniform mat4 world_matrix;
uniform mat4 model_matrix;
uniform float spacing;

void main() {
	gl_Position = position;
	gl_Position.x += gl_InstanceID * (1 + spacing);
	gl_Position.y = -position.y;
	gl_Position = world_matrix * model_matrix * gl_Position;

	vertex_uv.x = (position.x + index % 16) / 16;
	vertex_uv.y = (position.y + index / 16) / 16;
}

)glsl";

const char* text_fragment_shader = R"glsl(
#version 330

in vec2 vertex_uv;
out vec4 out_color;

uniform sampler2D font;

void main() {
	out_color = texture(font, vertex_uv);
}

)glsl";

const glm::vec4 model[] = {
	glm::vec4(0, 0, 0, 1),
	glm::vec4(1, 0, 0, 1),
	glm::vec4(1, 1, 0, 1),
	glm::vec4(0, 1, 0, 1)
};

GLuint Text::vao = 0;
GLuint Text::model_buffer = 0;
Program Text::program;
Texture Text::font;

Text::Text(std::string new_content, std::string instance_name) : Object(instance_name), spacing(0) {
	glGenBuffers(1, &string_buffer);
	setContent(new_content);
	model_matrix = glm::mat4(1);

	if (program.getId() == 0) {
		/*glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);*/
	
		glGenBuffers(1, &model_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(model), model, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
		//glVertexAttribDivisor(1, 1);

		font.load("rsrc/font.png", GL_NEAREST);
		program.load(text_vertex_shader, text_fragment_shader);

		glUniform1i(program.uniformLocation("font"), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, font.getId());
	}
};

void Text::setPosition(glm::vec2 position) {
	model_matrix[3][0] = position.x;
	model_matrix[3][1] = position.y;
}

void Text::setScale(glm::vec2 scale) {
	model_matrix[0][0] = scale.x;
	model_matrix[1][1] = scale.y;
}

void Text::setSpacing(float new_spacing) {
	spacing = new_spacing;
}

void Text::setContent(std::string new_content) {
	content = new_content;
	glBindBuffer(GL_ARRAY_BUFFER, string_buffer);
	glBufferData(GL_ARRAY_BUFFER, content.length(), content.c_str(), GL_STATIC_DRAW);
}

void Text::render() {
	if (!visible) {
		return;
	}

	glUseProgram(program.getId());
	//glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, string_buffer);
	//glVertexAttribIPointer(1, 1, GL_UNSIGNED_BYTE, 0, NULL);
	//glVertexAttribDivisor(1, 1);


	glUniform1f(program.uniformLocation("spacing"), spacing);
	glUniformMatrix4fv(program.uniformLocation("model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
	glUniformMatrix4fv(program.uniformLocation("world_matrix"), 1, GL_FALSE, (float*)&world_matrix);

	glUniform1i(program.uniformLocation("font"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font.getId());

	//glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, content.length());
}
