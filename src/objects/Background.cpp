#include "Background.h"

#include <iostream>

#include "glm/glm.hpp"

const char* vertex_shader = R"glsl(
//#version 100

attribute vec4 vertex_position;

varying vec3 position;

uniform mat4 world_matrix;
uniform vec3 offset;

void main() {
	gl_Position = world_matrix * (vertex_position + vec4(offset, 1));
	gl_Position.w = 1.0 + gl_Position.z;
	position = gl_Position.xyz;
}

)glsl";

const char* fragment_shader = R"glsl(
//#version 100

varying vec3 position;

uniform vec3 bg_color;

void main() {
	vec3 back_color = bg_color * 0.1;
	vec3 front_color = pow(bg_color, vec3(0.1));//vec3(0.3) + bg_color * 0.5;
	gl_FragColor.xyz = mix(front_color, back_color, sqrt(position.z));
	gl_FragColor.w = 1.0;
}

)glsl";

Background::Background(int div, std::string instance_name) : Object(instance_name), grid_div(div){
	model.push_back(glm::vec4(-2, -2, 1, 1));
	model.push_back(glm::vec4(-2, 2, 1, 1));
	model.push_back(glm::vec4(2, 2, 1, 1));
	model.push_back(glm::vec4(2, -2, 1, 1));

	for (int i = -div; i <= div; i++) {
		for (int j = 0; j < 4; j++) {
			model.push_back(glm::vec4(-2, 2.0 * i / div, j / 4.0, 1));
			model.push_back(glm::vec4(2, 2.0 * i / div, j / 4.0, 1));
			model.push_back(glm::vec4(2.0 * i / div, -2, j / 4.0, 1));
			model.push_back(glm::vec4(2.0 * i / div, 2, j / 4.0, 1));
		}
	}

	for (int i = -div; i <= div; i++) {
		for (int j = -div; j <= div; j++) {
			model.push_back(glm::vec4(2.0 * i / div, 4.0 * j / div, 0, 1));
			model.push_back(glm::vec4(2.0 * i / div, 4.0 * j / div, 1, 1));
		}
	}

	program.load(vertex_shader, fragment_shader);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &model_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(glm::vec4), &(model[0]), GL_STATIC_DRAW);

	color = glm::vec3(0, 0.2, 1);
}

void Background::render() {
	glUseProgram(program.getId());
	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniform3f(program.uniformLocation("bg_color"), color[0], color[1], color[2]);
	glUniformMatrix4fv(program.uniformLocation("world_matrix"), 1, GL_FALSE, (float*)&world_matrix);

	glUniform3f(program.uniformLocation("offset"), 0, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3f(program.uniformLocation("offset"), position[0], position[1], position[2]);
	glDrawArrays(GL_LINES, 4, model.size() - 4);
}

void Background::update() {
	time++;
	position.x = remainder(10 * sin(time * 0.0007), 4.0 / grid_div);
	position.y = remainder(5 * sin(time * 0.0031), 4.0 / grid_div);
	//position.z = remainder(sin(time * 0.011), 1 / 4.0);

	/*color.x = 0.5 * (1 + sin(time * 0.023));
	color.y = 0.5 * (1 + sin(time * 0.023 + 2));
	color.z = 0.5 * (1 + sin(time * 0.023 + 4));*/
}
