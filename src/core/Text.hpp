/*
 * Text related functions
 *
 * Functions for loading fonts and generating text textures.
 */

#pragma once

#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "core/Texture.hpp"


// Initialize the freetype library
bool initFreetype();

// Load a typeface
bool loadTypeface(std::string font_name);

// Renders text into a Texture
std::shared_ptr<Texture> renderText(std::string text);
