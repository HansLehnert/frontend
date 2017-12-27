#include "Background.h"

#include <iostream>

#include "glm/glm.hpp"


Background::Background(int div, std::string instance_name) : Object(instance_name), grid_div(div){
	model.push_back(glm::vec4(-4, -4, 1, 1));
	model.push_back(glm::vec4(-4, 4, 1, 1));
	model.push_back(glm::vec4(4, 4, 1, 1));
	model.push_back(glm::vec4(4, -4, 1, 1));

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

	program = Program::getProgram("background");

	glGenBuffers(1, &model_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(glm::vec4), &(model[0]), GL_STATIC_DRAW);

	color = glm::vec3(0, 0.2, 1);
}

void Background::render() {
	glUseProgram(program->getId());
	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniform3f(program->uniformLocation("bg_color"), color[0], color[1], color[2]);
	glUniformMatrix4fv(program->uniformLocation("world_matrix"), 1, GL_FALSE, (float*)&world_matrix);

	glUniform3f(program->uniformLocation("offset"), 0, 0, 0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3f(program->uniformLocation("offset"), position[0], position[1], position[2]);
	glDrawArrays(GL_LINES, 4, model.size() - 4);
}

void Background::update() {
	time++;
	position.x = remainder(10 * sin(time * 0.00021), 4.0 / grid_div);
	position.y = remainder(5 * sin(time * 0.00093), 4.0 / grid_div);
	//position.z = remainder(sin(time * 0.011), 1 / 4.0);

	/*color.x = 0.5 * (1 + sin(time * 0.023));
	color.y = 0.5 * (1 + sin(time * 0.023 + 2));
	color.z = 0.5 * (1 + sin(time * 0.023 + 4));*/
}
