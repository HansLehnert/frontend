#pragma once

#include <string>

#include "../gl_inc.h"

class Texture {
public:
	Texture();
	~Texture();

	int loadFile(std::string filename, GLuint filter = GL_LINEAR);
	void loadData(const unsigned char* data, int w, int h, int c = 4, GLuint filter = GL_LINEAR);

	//Texture copying
	Texture& operator=(Texture& a);


	bool isLoaded();
	int getWidth();
	int getHeight();
	GLuint getId();
	const unsigned char* getData();
private:
	bool loaded;

	GLuint id;
	unsigned char* image_buffer;
	int width;
	int height;

	void bufferTexture(GLuint filter);
};
