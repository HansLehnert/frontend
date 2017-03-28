#pragma once

#include <string>
#include "../gl_inc.h"
#include "glm/glm.hpp"
#include "core/Object.h"
#include "core/Texture.h"
#include "core/Program.h"

class Image: public Object {
public:
	Image(std::string = "");
	
	virtual void render();

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
