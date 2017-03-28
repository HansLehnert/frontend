#pragma once

#include <string>
#include "../gl_inc.h"
#include "glm/glm.hpp"
#include "core/Object.h"
#include "core/Texture.h"
#include "core/Program.h"

class Text: public Object {
public:
	Text(std::string = "");
	
	virtual void render();

	void setPosition(glm::vec2);
	void setScale(glm::vec2);
	void setSpacing(float);
	void setContent(std::string);
private:
	std::string content;
	float spacing;

	static GLuint vao;
	static GLuint model_buffer;
	static Program program;
	static Texture font;

	GLuint string_buffer = 0;
};
