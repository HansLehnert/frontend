/**
 * Plane graphical object
 *
 * To be used as base class for objects that are drawn as rectangles
 */

#pragma once

#include <string>
#include <memory>

#include "core/GraphicObject.hpp"

#include "gl_inc.h"
#include "glm/glm.hpp"


class Plane: public GraphicObject {
public:
    Plane(std::string instance_name = "");

    virtual void render();

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
    } origin;

protected:
    static GLuint model_buffer;
};
