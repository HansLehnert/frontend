#include "Text.h"

#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "core/Texture.h"
#include "core/Program.h"

Texture Text::font;

Text::Text(std::string text, std::string instance_name) : Image("", instance_name) {
	setText(text);

	if (!font.isLoaded()) {
		font.loadFile("rsrc/font.png");
	}
};


void Text::setText(std::string text) {
	content = text;

	int char_width = font.getWidth() / 16;
	int char_height = font.getHeight() / 16;
	int width = (char_width + 1) * content.length() - 1;
	int height = char_height;

	const unsigned char* font_data = font.getData();
	unsigned char* image_data = new unsigned char[width * height * 4];

	for (int i = 0; i < width * height * 4; i++)
		image_data[i] = 0;

	for (unsigned int i = 0; i < content.length(); i++) {
		for (int x = 0; x < char_width; x++) {
			for (int y = 0; y < char_height; y++) {
				int offset_image = (i * (char_width + 1) + x + y * width) * 4;
				int offset_font = ((content[i] % 16) * char_width + x + ((content[i] / 16) * char_height + y) * font.getWidth()) * 4;

				//Copy the 4 channels
				for (int c = 0; c < 4; c++) {
					image_data[offset_image + c] = font_data[offset_font + c];
				}
			}
		}
	}

	texture.loadData(image_data, width, height, 4, GL_NEAREST);
	setScale(glm::vec3(width, height, 0) * 0.01f);
	setPosition(glm::vec3(0));

	delete image_data;
}
