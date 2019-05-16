#include "ui/Marquee.hpp"


const int PIXEL_DENSITY = 480;


Marquee::Marquee(
        std::shared_ptr<Texture> initial_texture,
        std::string instance_name
) :
        Plane(instance_name)
{
    program = Program::getProgram("marquee");

    // Generate initial textures
    transition_mask = makeTransitionMask(PIXEL_DENSITY, PIXEL_DENSITY);

    previous_texture = initial_texture;
    current_texture = initial_texture;
    next_texture = initial_texture;

    // Store uniform locations for faster lookup
    uniform_previous_aspect_ratio = program->uniformLocation(
        "previous_aspect_ratio");
    uniform_current_aspect_ratio = program->uniformLocation(
        "current_aspect_ratio");
    uniform_progress = program->uniformLocation("progress");
    uniform_previous_image = program->uniformLocation("previous_image");
    uniform_current_image = program->uniformLocation("current_image");
    uniform_mask_image = program->uniformLocation("mask_image");

    // Transition variables
    transition_steps = 20;
    transition_progress = transition_steps;
}


void Marquee::update() {
    if (transition_progress < transition_steps) {
        transition_progress++;
    }
    else if (next_texture != current_texture) {
        // Restart the transition
        transition_progress = 0;

        // Set next image as current image
        previous_texture = current_texture;
        current_texture = next_texture;
    }
}


void Marquee::render() {
    glUniform1i(uniform_mask_image, 2);
    glUniform1i(uniform_previous_image, 0);
    glUniform1i(uniform_current_image, 1);
    glUniform1f(
        uniform_progress, (float)transition_progress / transition_steps);

    glUniform1f(
        uniform_previous_aspect_ratio, previous_texture->getAspectRatio());
    glUniform1f(
        uniform_current_aspect_ratio, current_texture->getAspectRatio());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, previous_texture->getId());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, current_texture->getId());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, transition_mask->getId());

    Plane::render();
}


void Marquee::updateImage(std::shared_ptr<Texture> next_image) {
    next_texture = next_image;
}


void Marquee::setSize(float width, float height) {
    scale.x = width;
    scale.y = height;
    transition_mask = makeTransitionMask(
        PIXEL_DENSITY * width, PIXEL_DENSITY * height);
}


std::shared_ptr<Texture> Marquee::makeTransitionMask(
        int width, int height) {
    int cell_size = 48;
    //int cell_growth = 10;

    std::vector<float> bitmap(width * height * 3);

    float min_val = 0;
    float max_val = 0;

    //Generate the pattern
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int y_ = height - y - 1; // inverted y coord

            // Transform coordinates to rotated plane
            int i = x + y;
            int j = x - y + (height / cell_size + 1) * cell_size;

            int ci = i / cell_size;
            int cj = j / cell_size;

            int row = ci - cj + (height / cell_size + 1);

            // Distances from center
            float val_i = abs(ci * cell_size - i + cell_size / 2);
            float val_j = abs(cj * cell_size - j + cell_size / 2);
            float val = -5 * row + std::max(val_i, val_j);

            max_val = std::max(max_val, val); // Used for normalizing
            min_val = std::min(min_val, val);

            if (row != 0) {
                bitmap[(x + width * y_) * 3] = val;
                bitmap[(x + width * y_) * 3 + 1] = 0;
                bitmap[(x + width * y_) * 3 + 2] = 1;
            }
            else {
                bitmap[(x + width * y_) * 3] = 100;
                bitmap[(x + width * y_) * 3 + 1] = 1;
                bitmap[(x + width * y_) * 3 + 2] = 0;
            }

        }
    }

    // Normalize
    for (int i = 0; i < bitmap.size() / 3; i++) {
        bitmap[3 * i] = (bitmap[3 * i] - min_val) / (max_val - min_val);
    }

    std::shared_ptr<Texture> mask = Texture::fromData(
        bitmap.data(), width, height, GL_NEAREST, GL_RGB, GL_FLOAT);

    return mask;
}
