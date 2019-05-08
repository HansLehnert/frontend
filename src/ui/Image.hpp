#pragma once

#include <string>
#include <memory>

#include "gl_inc.h"
#include "ui/Plane.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"

class Image: public Plane {
public:
    Image(std::shared_ptr<Texture> texture, std::string instance_name = "");

    virtual void render();

    // Scale the image size to fit a bounding box, while preserving the aspect
    // ratio of the texture
    void fitBounds(float max_width, float max_height);

    std::shared_ptr<Texture> texture;
    glm::vec4 tint;
    float opacity;
};
