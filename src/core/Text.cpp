#include "Text.hpp"

#include <string>
#include <iostream>

#include FT_GLYPH_H

#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"


FT_Library ft_library;
FT_Face ft_face;


bool initFreetype() {
    FT_Error ft_error;

    ft_error = FT_Init_FreeType(&ft_library);
    if (ft_error) {
        std::cout
            << "[Text]\tFailed to initialize Freetype library" << std::endl;
        return false;
    }
    else {
        return true;
    }
}


bool loadTypeface(std::string font_name) {
    FT_Error ft_error;

    ft_error = FT_New_Face(ft_library, font_name.c_str(), 0, &ft_face);
    if (ft_error) {
        std::cout
            << "[Text]\tFailed to load " << font_name << " font. "
            << "Error " << ft_error << "."
            << std::endl;
        return false;
    }
    else {
        FT_Set_Pixel_Sizes(ft_face, 0, 64);
        return true;
    }
}


std::shared_ptr<Texture> renderText(std::string text, TextFormat format) {
    FT_Error ft_error;

    FT_Glyph* glyphs = new FT_Glyph[text.length()];
    int* glyph_pos = new int[text.length()];

    unsigned int glyph_index;
    unsigned int previous_index = 0;

    unsigned int width = 0;
    unsigned int height = ft_face->size->metrics.height >> 6;
    int offset_x;
    int offset_y = ft_face->size->metrics.ascender >> 6;

    FT_Vector pen;
    pen.x = 0;

    for (unsigned int n = 0; n < text.length(); n++) {
        glyph_index = FT_Get_Char_Index(ft_face, text[n]);

        FT_Vector kerning;
        FT_Get_Kerning(ft_face, previous_index, glyph_index, FT_KERNING_DEFAULT, &kerning);

        ft_error = FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT);
        if (ft_error)
            continue;

        ft_error = FT_Get_Glyph(ft_face->glyph, &glyphs[n]);
        if (ft_error)
            continue;
        FT_BBox bounding_box;
        FT_Glyph_Get_CBox(glyphs[n], FT_GLYPH_BBOX_PIXELS, &bounding_box);

        if (n == 0) {
            offset_x = -bounding_box.xMin;
            width += -bounding_box.xMin;
        }

        if (n == text.length() - 1) {
            width += bounding_box.xMax + 1;
        }

        width += (ft_face->glyph->advance.x >> 6) + (kerning.x >> 6);

        pen.x += -kerning.x >> 6;
        glyph_pos[n] = pen.x;
        pen.x += ft_face->glyph->advance.x >> 6;

        previous_index = glyph_index;
    }

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

    std::shared_ptr<Texture> texture = Texture::fromData(
        bitmap, width, height, 4, GL_LINEAR);

    delete[] bitmap;
    delete[] glyphs;
    delete[] glyph_pos;

    return texture;
}
