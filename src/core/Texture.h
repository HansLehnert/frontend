#pragma once

#include <string>

#include "../gl_inc.h"

class Texture {
public:
	Texture();
	~Texture();

	int loadFile(std::string, GLuint = GL_LINEAR);
	void loadData(const unsigned char*, int, int, int = 4, GLuint = GL_LINEAR);

	bool isLoaded();
	GLuint getId();
	int getWidth();
	int getHeight();
	const unsigned char* getData();
private:
	bool loaded;

	GLuint id;
	unsigned char* image_buffer;
	int width;
	int height;

	void bufferTexture(GLuint);
};
