#pragma once

#include "core/GraphicObject.hpp"

#include <vector>

#include "gl_inc.h"
#include "glm/glm.hpp"

class Background : public GraphicObject {
public:
    Background(std::string = "");

    void updateSelf() override;
    void render();

    /**
     * @brief Adjust the size to fit the size of the screen space
     *
     * @param grid_size Separation between grid lines
     */
    void fit(float width, float height, float grid_spacing);

    glm::vec3 background_color;
    glm::vec3 accent_color;

    glm::vec3 scroll_offset;

private:
    int time;

    // Spacing of the grid lines
    glm::vec2 spacing;

    // Model
    GLuint model_buffer;
    unsigned int model_size;

    // Uniform locations
    GLuint background_color_loc;
    GLuint accent_color_loc;
    GLuint offset_loc;
};
