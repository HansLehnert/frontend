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

protected:
    static GLuint model_buffer;
};
