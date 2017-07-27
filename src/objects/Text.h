#pragma once

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../gl_inc.h"
#include "glm/glm.hpp"

#include "Image.h"
#include "core/Object.h"
#include "core/Texture.h"

class Text: public Image {
public:
	Text(std::string = "", std::string = "");

	void setText(std::string);
	std::string getText();

private:
	std::string text;

	void updateTexture();

	//////////////////////////////////////////////////////////
	//Freetype
	//////////////////////////////////////////////////////////
public:
	static bool initFreetype();
	static bool loadTypeface(std::string);

private:
	static FT_Library ft_library;
	static FT_Face ft_face;
};
