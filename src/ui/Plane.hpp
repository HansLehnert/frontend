/**
 * Plane graphical object
 *
 * To be used as base class for objects that are drawn as rectangles
 */

#pragma once

#include <string>
#include <memory>

#include "core/GraphicObject.hpp"
#include "gfx/Mesh.hpp"

#include "gl_inc.h"
#include "glm/glm.hpp"


class Plane: public GraphicObject {
public:
    enum class Origin {
        TOP_LEFT,
        TOP,
        TOP_RIGHT,
        LEFT,
        CENTER,
        RIGHT,
        BOTTOM_LEFT,
        BOTTOM,
        BOTTOM_RIGHT
    };

    Plane(std::string name = "");

    void render() override;

    const glm::vec2& size() const { return size_; }
    void setSize(glm::vec2 size);

    Origin origin() const { return origin_; }
    void setOrigin(Origin origin) { origin_ = origin; }

protected:
    static Mesh mesh_;

private:
    Origin origin_;
    glm::vec2 size_;
};
