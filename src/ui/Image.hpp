#pragma once

#include <string>
#include <memory>

#include "gl_inc.h"
#include "ui/Plane.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"

class Image: public Plane {
public:
    enum class FillMode {
        Fit,
        Stretch,
    };

    Image(std::shared_ptr<Texture> texture, std::string instance_name = "");

    virtual void render() const;

    const Texture& texture() const { return *texture_; }
    void setTexture(std::shared_ptr<Texture> texture) { texture_ = texture; }

    FillMode  fill_mode_;
    glm::vec4 tint_color_;
    float     opacity_;

private:
    std::shared_ptr<Texture> texture_;
};
