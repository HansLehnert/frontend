#include "Frame.hpp"

#include "gl_inc.h"

#include "core/Program.hpp"


Frame::Frame(std::string name) : Plane(name), border_color_(1) {
    program_ = Program::getProgram("frame");
}


void Frame::render() const {
    if (fill_color_.a > 0) {
        glUniform4f(
            program().uniformLocation("color"),
            fill_color_.r, fill_color_.g, fill_color_.b, fill_color_.a
        );

        Plane::draw(true);
    }

    if (border_color_.a > 0) {
        glUniform4f(
            program().uniformLocation("color"),
            border_color_.r, border_color_.g, border_color_.b, border_color_.a
        );

        Plane::draw(false);
    }
}
