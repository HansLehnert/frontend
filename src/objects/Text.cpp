#include "Text.h"

#include <string>
#include <iostream>

#include FT_GLYPH_H

#include "glm/glm.hpp"
#include "core/Texture.h"
#include "core/Program.h"


FT_Library Text::ft_library;
FT_Face Text::ft_face;


Text::Text(std::string text, std::string instance_name) : Image("", instance_name) {
	setText(text);
};



bool Text::initFreetype() {
	FT_Error ft_error;

	ft_error = FT_Init_FreeType(&ft_library);
	if (ft_error) {
		std::cout << "[Text]\tFailed to initialize Freetype library" << std::endl;
		return false;
	}
	else {
		return true;
	}
}


bool Text::loadTypeface(std::string font_path) {
	FT_Error ft_error;

	ft_error = FT_New_Face(ft_library, font_path.c_str(), 0, &ft_face);
	if (ft_error) {
		std::cout << "[Text]\tFailed to load " << font_path << "font." << std::endl;
		return false;
	}
	else {
		FT_Set_Pixel_Sizes(ft_face, 0, 64);
		return true;
	}
}


void Text::setText(std::string new_text) {
	text = new_text;
	updateTexture();

	/*int char_width = font.getWidth() / 16;
	int char_height = font.getHeight() / 16;
	int width = (char_width + 1) * text.length() - 1;
	int height = char_height;

	const unsigned char* font_data = font.getData();
	unsigned char* image_data = new unsigned char[width * height * 4];

	for (int i = 0; i < width * height * 4; i++)
		image_data[i] = 0;

	for (unsigned int i = 0; i < text.length(); i++) {
		for (int x = 0; x < char_width; x++) {
			for (int y = 0; y < char_height; y++) {
				int offset_image = (i * (char_width + 1) + x + y * width) * 4;
				int offset_font = ((text[i] % 16) * char_width + x + ((text[i] / 16) * char_height + y) * font.getWidth()) * 4;

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

	delete image_data;*/
}


void Text::updateTexture() {
	FT_Error ft_error;

	FT_Glyph* glyphs = new FT_Glyph[text.length()];
	int* glyph_pos = new int[text.length()];

	unsigned int glyph_index;
	//unsigned int previous_index = 0;

	unsigned int width = 0;
	unsigned int height = ft_face->size->metrics.height >> 6;
	int offset_x;
	int offset_y = ft_face->size->metrics.ascender >> 6;

	FT_Vector pen;
	pen.x = 0;

	for (unsigned int n = 0; n < text.length(); n++) {
		glyph_index = FT_Get_Char_Index(ft_face, text[n]);

		//FT_Vector kerning;
		//FT_Get_Kerning(ft_face, previous_index, glyph_index, FT_KERNING_DEFAULT, &kerning);

		ft_error = FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT);
		if (ft_error)
			continue;

		ft_error = FT_Get_Glyph(ft_face->glyph, &glyphs[n]);
		if (ft_error)
			continue;

		//previous = glyph_index;
		FT_BBox bounding_box;
		FT_Glyph_Get_CBox(glyphs[n], FT_GLYPH_BBOX_PIXELS, &bounding_box);

		if (n == 0) {
			offset_x = -bounding_box.xMin >> 6;
			width += offset_x;
		}
		else if (n == text.length() - 1) {
			width += (bounding_box.xMax >> 6) + 1;
		}

		glyph_pos[n] = pen.x;
		pen.x += ft_face->glyph->advance.x >> 6;
	}
	width += pen.x;

	unsigned char* bitmap = new unsigned char[width * height * 4];
	for (unsigned int i = 0; i < width * height; i++) {
		bitmap[i *  4] = 255;
		bitmap[i *  4 + 1] = 255;
		bitmap[i *  4 + 2] = 255;
		bitmap[i *  4 + 3] = 0;
	}

	for (unsigned int n = 0; n < text.length(); n++) {
		pen.x = offset_x + glyph_pos[n];
		pen.y = offset_y;

		FT_Glyph_To_Bitmap(&glyphs[n], FT_RENDER_MODE_NORMAL, &pen, 0);
		FT_BitmapGlyph bit = (FT_BitmapGlyph)glyphs[n];

		for (unsigned int i = 0; i < bit->bitmap.width; i++) {
			for (unsigned int j = 0; j < bit->bitmap.rows; j++) {
				unsigned int pixel_offset = (i + pen.x + bit->left + width * (j + pen.y - bit->top)) * 4 + 3;
				if (pixel_offset < width * height * 4)
					bitmap[pixel_offset] = bit->bitmap.buffer[i + bit->bitmap.pitch * j];
			}
		}

		FT_Done_Glyph(glyphs[n]);
	}

	texture.loadData(bitmap, width, height, 4, GL_LINEAR);
	setScale(glm::vec3(width, height, 0) * 0.001f);
	setPosition(glm::vec3(0));

	delete bitmap;
	delete glyphs;
	delete glyph_pos;
}