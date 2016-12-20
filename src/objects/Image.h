#pragma once

#include <string>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "core/Object.h"
#include "core/Texture.h"
#include "core/Program.h"

class Image: public Object {
public:
	Image(std::string = "");
	
	virtual void render();

	void setPosition(glm::vec2);
	void setScale(glm::vec2);
	int setContent(std::string);
	void setSize(glm::vec2);
private:
	std::string content;
	float shape;

	static GLuint model_buffer;
	static Program program;
	
	Texture texture;

	GLuint string_buffer = 0;
};