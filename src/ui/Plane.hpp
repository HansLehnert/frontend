
#pragma once

#include <string>
#include <memory>

#include "core/GraphicObject.hpp"
#include "gfx/Mesh.hpp"

#include "gl_inc.h"
#include "glm/glm.hpp"


// Plane graphic object, to be used for rectangular objects
class Plane: public GraphicObject {
public:
    // Describes the transformation origin of the plane object when rendered
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

    // Can be overriden by derived objects that need to resize their children
    const glm::vec2& size() const { return size_; }
    virtual void setSize(glm::vec2 size) { size_ = size; }

protected:
    void draw(bool fill) const;

    const Mesh& mesh() { return mesh_; }

public:
    Origin origin_;

private:
    glm::vec2 size_;
    static Mesh mesh_;
};
