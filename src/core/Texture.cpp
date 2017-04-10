#include "core/Texture.h"

#include <iostream>
#include <cstring>

#include <SOIL/SOIL.h>

Texture::Texture() : loaded(false) {
	glGenTextures(1, &id);

	return;
}


Texture::~Texture() {
	glDeleteTextures(1, &id);
	SOIL_free_image_data(image_buffer);
	return;
}


int Texture::loadFile(std::string filename, GLuint filter) {
	image_buffer = SOIL_load_image(filename.c_str(), &width, &height, NULL, SOIL_LOAD_RGBA);

	if (image_buffer) {
		glBindTexture(GL_TEXTURE_2D, id);

		bufferTexture(filter);

		return 1;
	}
	else {
		std::cout << "[Texture]\tFailed to load " << filename << std::endl;
		return 0;
	}
}


void Texture::loadData(const unsigned char* data, int w, int h, int c, GLuint filter) {
	image_buffer = new unsigned char[w * h * c];
	memcpy(image_buffer, data, w * h * c);

	width = w;
	height = h;

	bufferTexture(filter);

	loaded = true;
}


bool Texture::isLoaded() {
	return loaded;
}


void Texture::bufferTexture(GLuint filter) {
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
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

const unsigned char* Texture::getData() {
	return image_buffer;
}