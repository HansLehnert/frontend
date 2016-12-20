#pragma once

#include <GL/glew.h>

class Texture {
public:
	Texture();
	~Texture();

	int load(const char*, GLuint = GL_LINEAR);
	GLuint getId();
	int getWidth();
	int getHeight();
private:
	GLuint id;
	int width;
	int height;
};