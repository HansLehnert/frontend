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
        FIT = 0,
        STRETCH = 1,
    };

    Image(std::shared_ptr<Texture> texture, std::string instance_name = "");

    virtual void render() const;

    void setSize(glm::vec2 size);

    const Texture& texture() const { return *texture_; }
    void setTexture(std::shared_ptr<Texture> texture);

private:
    void resize();

public:
    FillMode  fill_mode_;
    glm::vec4 tint_color_;
    float     opacity_;

private:
    std::shared_ptr<Texture> texture_;
    glm::vec2 target_size_;
};
