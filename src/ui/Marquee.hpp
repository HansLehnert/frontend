#pragma once

#include "ui/Plane.hpp"
#include "ui/Label.hpp"

#include "core/Texture.hpp"

class Marquee : public Plane {
public:
    Marquee(
        std::shared_ptr<Texture> initial_texture,
        std::string instance_name = ""
    );

    void render() const override;

    // Start the transition to a new image or enqueue an image to transition to
    void updateImage(std::shared_ptr<Texture> next_image);

    void setSize(glm::vec2 size) override;


    std::shared_ptr<Texture> makeTransitionMask(int width, int height);

protected:
    void updateSelf() override;

private:
    unsigned int transition_progress;
    unsigned int transition_steps;

    GLuint model_buffer;

    // Uniform location
    GLuint uniform_progress;
    GLuint uniform_mask_image;
    GLuint uniform_previous_image;
    GLuint uniform_current_image;
    GLuint uniform_previous_aspect_ratio;
    GLuint uniform_current_aspect_ratio;

    // Textures
    std::shared_ptr<Texture> transition_mask;
    std::shared_ptr<Texture> previous_texture;
    std::shared_ptr<Texture> current_texture;
    std::shared_ptr<Texture> next_texture;

    std::shared_ptr<Label> label;
};
