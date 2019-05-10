#include "Text.hpp"

#include <string>
#include <vector>
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


std::shared_ptr<Texture> renderText(std::string text) {
    FT_Error ft_error;

    std::vector<FT_Glyph> glyphs(text.length());
    std::vector<FT_Vector> glyph_positions(text.length());

    unsigned int bitmap_width;
    unsigned int bitmap_height = ft_face->size->metrics.height >> 6;

    FT_UInt16 previous_glyph_index; // Used for kerning

    FT_Pos advance;

    for (unsigned int n = 0; n < text.length(); n++) {
        // Load the glyph
        FT_UInt glyph_index = FT_Get_Char_Index(ft_face, text[n]);

        ft_error = FT_Load_Glyph(ft_face, glyph_index, FT_LOAD_DEFAULT);
        if (ft_error)
            continue;

        ft_error = FT_Get_Glyph(ft_face->glyph, &glyphs[n]);
        if (ft_error)
            continue;

        // Compute glyph position
        if (n == 0) {
            FT_BBox bounding_box;
            FT_Glyph_Get_CBox(glyphs[n], FT_GLYPH_BBOX_GRIDFIT, &bounding_box);

            glyph_positions[n].x = -bounding_box.xMin;
            glyph_positions[n].y = -ft_face->size->metrics.ascender;
        }
        else {
            glyph_positions[n] = glyph_positions[n - 1];
            glyph_positions[n].x += advance;

            if (FT_HAS_KERNING(ft_face)) {
                FT_Vector kerning;
                FT_Get_Kerning(
                    ft_face,
                    previous_glyph_index,
                    glyph_index,
                    FT_KERNING_DEFAULT,
                    &kerning
                );

                glyph_positions[n].x += kerning.x;
            }
        }

        // On the last glyph, the bounding box is used to calculate the total
        // bitmap width
        if (n == text.length() - 1) {
            FT_BBox bounding_box;
            FT_Glyph_Get_CBox(glyphs[n], FT_GLYPH_BBOX_GRIDFIT, &bounding_box);

            bitmap_width = glyph_positions[n].x + bounding_box.xMax;
            bitmap_width = (bitmap_width + 63) >> 6; // Ceil divide
        }

        advance = ft_face->glyph->metrics.horiAdvance;
        previous_glyph_index = glyph_index;
    }


    std::vector<unsigned char> bitmap(bitmap_width * bitmap_height * 4);
    // The bitmap is initialized as transparent white. The alpha channel will
    // hold the glyph bitmaps
    for (unsigned int i = 0; i < bitmap.size() / 4; i++) {
        bitmap[i *  4] = 255;
        bitmap[i *  4 + 1] = 255;
        bitmap[i *  4 + 2] = 255;
        bitmap[i *  4 + 3] = 0;
    }

    for (unsigned int n = 0; n < text.length(); n++) {
        // Get the glyph bitmap
        ft_error = FT_Glyph_To_Bitmap(
            &glyphs[n], FT_RENDER_MODE_NORMAL, &glyph_positions[n], 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyphs[n];

        if (ft_error) {
            FT_Done_Glyph(glyphs[n]);
            continue;
        }

        // Copy glyph to the texture bitmap
        int k = 0;
        for (unsigned int i = 0; i < bitmap_glyph->bitmap.rows; i++) {
            for (unsigned int j = 0; j < bitmap_glyph->bitmap.width; j++) {
                unsigned int dst_index = 3 + 4 * (j
                    + bitmap_glyph->left
                    + bitmap_width * (i - bitmap_glyph->top)
                );

                if (dst_index < bitmap.size()) {
                    bitmap[dst_index] = bitmap_glyph->bitmap.buffer[
                        j + bitmap_glyph->bitmap.pitch * i];
                }
            }
        }

        FT_Done_Glyph(glyphs[n]);
    }

    std::shared_ptr<Texture> texture = Texture::fromData(
        bitmap.data(), bitmap_width, bitmap_height, 4, GL_LINEAR);

    return texture;
}
