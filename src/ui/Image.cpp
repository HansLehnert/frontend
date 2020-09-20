#include "ui/Image.hpp"

#include <string>
#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"


Image::Image(std::shared_ptr<Texture> texture, std::string name) :
        Plane(name),
        fill_mode_(Image::FillMode::FIT),
        tint_color_(0),
        opacity_(1),
        texture_(texture)
{
    program_ = Program::getProgram("image");

    if (texture_ == nullptr)
        texture_ = Texture::blankTexture();
};


void Image::setSize(glm::vec2 size) {
    target_size_ = size;
    resize();
}


void Image::setTexture(std::shared_ptr<Texture> texture) {
    texture_ = texture;
    resize();
}


void Image::resize() {
    // TODO: Maybe mobe this to Plane
    glm::vec2 effective_size = target_size_;

    if (fill_mode_ == FillMode::FIT) {
        float target_aspect_ratio = target_size_.x / target_size_.y;
        float texture_aspect_ratio = texture_->aspectRatio();
        if (target_aspect_ratio > texture_aspect_ratio) {
            effective_size.x = target_size_.y * texture_aspect_ratio;
            effective_size.y = target_size_.y;
        }
        else {
            effective_size.x = target_size_.x;
            effective_size.y = target_size_.x / texture_aspect_ratio;
        }
    }

    Plane::setSize(effective_size);
}


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
