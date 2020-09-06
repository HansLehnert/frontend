#include "ui/Image.hpp"

#include <string>
#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"


Image::Image(std::shared_ptr<Texture> texture, std::string name) :
        Plane(name),
        fill_mode_(Image::FillMode::Fit),
        tint_color_(0),
        opacity_(1),
        texture_(texture)
{
    program_ = Program::getProgram("image");
};


void Image::render() const {
    glUniform4fv(program().uniformLocation("tint"), 1, (float*)&tint_color_);
    glUniform1f(program().uniformLocation("opacity"), opacity_);

    glUniform1i(program().uniformLocation("image"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->getId());

    Plane::draw(true);
}


/*
void Image::fitBounds(float max_width, float max_height) {
    float bounds_ratio = max_width / max_height;
    float texture_ratio = (float)texture->getWidth() / texture->getHeight();

    if (bounds_ratio < texture_ratio) {
        scale.x = max_width;
        scale.y = max_width / texture_ratio;
    }
    else {
        scale.x = max_height * texture_ratio;
        scale.y = max_height;
    }
}*/
