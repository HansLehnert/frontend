#include "Plane.hpp"

#include <array>

#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"


// Model data
const glm::vec4 vertex_data[] = {
    glm::vec4(0, 0, 0, 1),
    glm::vec4(1, 0, 0, 1),
    glm::vec4(1, 1, 0, 1),
    glm::vec4(0, 1, 0, 1),
    glm::vec4(0, 0, 0, 1),
};


GLuint Plane::model_buffer = 0;


Plane::Plane(std::string instance_name) :
        GraphicObject(instance_name)
{
    // Initialize the quad model buffer for rendering the image
    if (model_buffer == 0) {
        glGenBuffers(1, &model_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
        glBufferData(
            GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    }

    program = Program::getProgram("image");
};


void Plane::render() {
    glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glUniformMatrix4fv(program->uniformLocation("model_matrix"), 1, GL_FALSE, (float*)&model_matrix);
    glUniformMatrix4fv(program->uniformLocation("world_matrix"), 1, GL_FALSE, (float*)&world_matrix);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
