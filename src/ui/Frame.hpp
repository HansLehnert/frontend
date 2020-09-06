#pragma once

#include "glm/glm.hpp"

#include "ui/Plane.hpp"

// A solid color rectangle with a border
class Frame : public Plane {
public:
    Frame(const std::string name = "");

    void render() const override;

public:
    glm::vec4 fill_color_;
    glm::vec4 border_color_;
};
