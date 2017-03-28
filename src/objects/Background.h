#pragma once

#include <vector>

#include "gl_inc.h"
#include "glm/glm.hpp"
#include "core/Program.h"
#include "core/Object.h"

class Background : public Object {
public:
	Background(int);

	virtual void render();
	virtual void update();

private:
	int time;
	int grid_div;
	GLuint model_buffer;
	std::vector<glm::vec4> model;
	glm::vec3 position;
	glm::vec3 color;

	Program program;
};
