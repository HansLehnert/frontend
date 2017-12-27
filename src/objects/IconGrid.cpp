#include "IconGrid.h"

#include "glm/glm.hpp"

//////////////////////////////////////////////////////////
//Model
//////////////////////////////////////////////////////////

const glm::vec4 model[] = {
	glm::vec4(0, 0, -1, 1),
	glm::vec4(0, 1, -1, 1),
	glm::vec4(1, 1, -1, 1),
	glm::vec4(1, 0, -1, 1),
	glm::vec4(0, 0, -1, 1)
};

//////////////////////////////////////////////////////////
//Member functions
//////////////////////////////////////////////////////////

IconGrid::IconGrid(std::string instance_name) :
	Object(instance_name)
{
	//Icon properties
	//this should be able to be modified in the future
	columns = 3;
	icon_ratio = 3.0 / 4.0;
	icon_margin = 0.1;
	icon_width = (2.0 - icon_margin * (columns - 1)) / columns;
	icon_height = icon_width * icon_ratio;

	//Calculate icon scaling;
	model_matrix[0][0] = icon_width;
	model_matrix[1][1] = icon_height;

	//Generate model buffer
	glGenBuffers(1, &model_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model), model, GL_STATIC_DRAW);

	//Get program and uniforms
	image_program = Program::getProgram("image");
	image_uniform_world_matrix = image_program->uniformLocation("world_matrix");
	image_uniform_model_matrix = image_program->uniformLocation("model_matrix");
	image_uniform_texture = image_program->uniformLocation("image");
}


void IconGrid::update() {

}


void IconGrid::render() {
	glUseProgram(image_program->getId());

	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(image_uniform_world_matrix, 1, GL_FALSE, (float*)&world_matrix);

	glUniform1i(image_uniform_texture, 0);
	glActiveTexture(GL_TEXTURE0);

	//Draw the icons
	for (unsigned int i = 0; i < icons.size(); i++) {
		//Calculate icon position
		int row = i / columns;
		int col = i % columns;
		model_matrix[3][0] = (icon_width + icon_margin) * col - 1;
		model_matrix[3][1] = -(icon_height + icon_margin) * row;


		glUniformMatrix4fv(image_uniform_model_matrix, 1, GL_FALSE, (float*)&model_matrix);

		if (icons[i].icon_texture.isLoaded())
			glBindTexture(GL_TEXTURE_2D, icons[i].icon_texture.getId());

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}


void IconGrid::setNumIcons(int n) {
	icons.resize(n);
}


bool IconGrid::setIconId(unsigned int index, std::string id) {
	if (index >= icons.size())
		return false;

	icons[index].id = index;

	return true;
}


bool IconGrid::setIconName(unsigned int index, std::string name) {
	if (index >= icons.size())
		return false;

	icons[index].name = name;
	icons[index].name_texture.setText(name);

	return true;
}


bool IconGrid::setIconGraphic(unsigned int index, std::string filename) {
	if (index >= icons.size())
		return false;

	icons[index].icon_texture.loadFile(filename);

	return true;
}


void IconGrid::handleInput(Event event) {

}