#include "Marquee.h"

#include <iostream>


//////////////////////////////////////////////////////////
//Model
//////////////////////////////////////////////////////////

const glm::vec4 model[] = {
	glm::vec4(-1, -1, 0, 1),
	glm::vec4(-1, 1, 0, 1),
	glm::vec4(1, 1, 0, 1),
	glm::vec4(1, -1, 0, 1)
};


//////////////////////////////////////////////////////////
//Member functions
//////////////////////////////////////////////////////////


Marquee::Marquee(std::string instance_name) :
	GraphicObject(instance_name)
{
	glGenBuffers(1, &model_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model), model, GL_STATIC_DRAW);

	program = Program::getProgram("marquee");

	//Position and scale
	dimension_aspect = 0.5;
	setScale(glm::vec3(1, dimension_aspect, 1));


	//Generate initial textures
	generateTransitionMask(512, 512 * dimension_aspect);
	current_image = transition_mask;
	next_aspect = 1;
	next_image = transition_mask;


	//Uniform locations
	uniform_model_matrix = program->uniformLocation("model_matrix");
	uniform_world_matrix = program->uniformLocation("world_matrix");
	uniform_current_aspect = program->uniformLocation("current_aspect");
	uniform_next_aspect = program->uniformLocation("next_aspect");
	uniform_progress = program->uniformLocation("progress");
	uniform_current_image = program->uniformLocation("current_image");
	uniform_next_image = program->uniformLocation("next_image");
	uniform_mask_image = program->uniformLocation("mask_image");

	//Transition variables
	queued_image = false;
	progress = 1;
	transition_speed = 0.05;
}


void Marquee::update() {
	if (progress < 1) {
		progress += transition_speed;
	}
	else if (progress >= 1 && queued_image) {
		//Transition end
		queued_image = false;
		progress = 0;

		//Set next image as current image
		current_image = next_image;
		current_aspect = next_aspect;

		//Load next image
		if (!next_image.loadFile(queued_filename)) {
			next_image = transition_mask;
		}
		next_aspect = dimension_aspect / next_image.getHeight() * next_image.getWidth();
	}
}


void Marquee::render() {
	glUseProgram(program->getId());

	glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glUniformMatrix4fv(uniform_model_matrix, 1, GL_FALSE, (float*)&model_matrix);
	glUniformMatrix4fv(uniform_world_matrix, 1, GL_FALSE, (float*)&world_matrix);

	glUniform1f(uniform_progress, progress);
	glUniform1i(uniform_current_image, 0);
	glUniform1i(uniform_next_image, 1);
	glUniform1i(uniform_mask_image, 2);

	glUniform1f(uniform_current_aspect, current_aspect);
	glUniform1f(uniform_next_aspect, next_aspect);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, current_image.getId());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, next_image.getId());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, transition_mask.getId());

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void Marquee::updateImage(std::string filename) {
	queued_filename = filename;
	queued_image = true;
}


//Generates the pattern for the transition effect between two images
void Marquee::generateTransitionMask(int width, int height) {
	int cell_size = 64;
	//int cell_growth = 10;

	unsigned char* bitmap = new unsigned char[width * height * 4];

	int max_val = 0;

	//Generate the pattern
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int y_ = height - y - 1; //inverted y coord

			//Transform coordinates to rotated plane
			int i = x + y;
			int j = x - y + (height / cell_size + 1) * cell_size;

			int ci = i / cell_size;
			int cj = j / cell_size;

			int row = ci - cj + (height / cell_size + 1);

			//Distances from center
			unsigned char val_i = abs(ci * cell_size - i + cell_size / 2);
			unsigned char val_j = abs(cj * cell_size - j + cell_size / 2);
			unsigned char val = 5 * row + (val_i > val_j ? val_i : val_j);

			max_val = val > max_val ? val : max_val; //Used for normalizing later

			bitmap[(x + width * y_) * 4] = val;
			if (row != 0)
				bitmap[(x + width * y_) * 4 + 3] = 255;
			else
				bitmap[4 * x + 4 * width * y + 3] = 0;
		}
	}

	//Normalize
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int val = bitmap[4 * x + 4 * width * y];
			val = (unsigned char)((int)val * 255 / max_val);

			bitmap[4 * x + 4 * width * y] = val;
			bitmap[4 * x + 4 * width * y + 1] = val;
			bitmap[4 * x + 4 * width * y + 2] = val;
		}
	}

	transition_mask.loadData(bitmap, width, height);

	delete bitmap;
}
