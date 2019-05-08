#include "ui/Image.hpp"

#include <string>
#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"


Image::Image(std::shared_ptr<Texture> texture, std::string instance_name) :
        Plane(instance_name),
        texture(texture),
        opacity(1),
        tint(0)
{
    program = Program::getProgram("image");
};


void Image::render() {
    glUniform4fv(program->uniformLocation("tint"), 1, (float*)&tint);
    glUniform1f(program->uniformLocation("opacity"), opacity);

    glUniform1i(program->uniformLocation("image"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getId());

    Plane::render();
}


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
}
