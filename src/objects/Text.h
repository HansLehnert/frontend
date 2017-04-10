#pragma once

#include <string>

#include "../gl_inc.h"
#include "glm/glm.hpp"

#include "Image.h"
#include "core/Object.h"
#include "core/Texture.h"

class Text: public Image {
public:
	Text(std::string = "", std::string = "");

	void setText(std::string);
private:
	std::string content;

	static Texture font;
};
