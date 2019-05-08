#include "ui/Image.hpp"

#include <string>
#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"


Image::Image(std::shared_ptr<Texture> texture, std::string instance_name) :
        Plane(instance_name),
        texture(texture)
{
    program = Program::getProgram("image");
};


void Image::render() {
    glUniform1i(program->uniformLocation("image"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getId());

    Plane::render();
}
