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

    std::shared_ptr<Texture> texture;
};
