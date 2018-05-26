#pragma once

#include <string>

#include "gl_inc.h"

class Texture {
public:
	Texture();
	Texture(const Texture& texture);
	Texture(Texture&& texture);
	~Texture();

	/**
	 * @brief Read texture data from file.
	 */
	int loadFile(std::string filename, GLuint filter = GL_LINEAR);

	/**
	 * @brief Load texture from raw data.
	 */
	void loadData(
		const unsigned char* data,
		int width,
		int height,
		int channels = 4,
		GLuint filter = GL_LINEAR);

	/**
	 * @brief Copy texture
	 *
	 * Creates a new GL image buffer with identical data.
	 */
	Texture& operator=(const Texture& a);


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
