#include "core/Texture.h"

#include <iostream>

#include <SOIL/SOIL.h>

Texture::Texture() {
	return;
}

Texture::~Texture() {
	return;
}

int Texture::load(const char* filename, GLuint filter) {
	unsigned char* image_buffer = NULL;
	image_buffer = SOIL_load_image(filename, &width, &height, NULL, SOIL_LOAD_RGBA);

	if (image_buffer) {
		if (id == 0)
			glGenTextures(1, &id);

		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);

		SOIL_free_image_data(image_buffer);
		return 1;
	}
	else {
		std::cout << "[Texture]\tFailed to load " << filename << std::endl;
		return 0;
	}
}

GLuint Texture::getId() {
	return id;
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}
