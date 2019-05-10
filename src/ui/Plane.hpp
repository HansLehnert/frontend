/*
 * PLane graphical object
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
        TOP_CENTER,
        TOP_RIGHT,
        CENTER_LEFT,
        CENTER,
        CENTER_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT
    } origin;

protected:
    static GLuint model_buffer;
};
