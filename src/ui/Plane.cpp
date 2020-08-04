#include "Plane.hpp"

#include <array>

#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"
#include "gfx/Mesh.hpp"


// Model data
const std::array<Vertex, 5> base_vertex_data = {
    0, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 1, 0,
    1, 1, 0, 1, 1, 1,
    0, 1, 0, 1, 0, 1,
    0, 0, 0, 1, 0, 0,
};


Mesh Plane::mesh_;


Plane::Plane(std::string instance_name) :
        GraphicObject(instance_name),
        origin_(Plane::Origin::TOP_LEFT)
{
    // Initialize the quad model buffer for rendering the image
    if (!mesh_.hasVertexData()) {
        // Generate vertex data for each origin position
        std::array<Vertex, base_vertex_data.size() * 9> vertex_data;
        for (int i = 0; i < 9; i++) {
            for (unsigned int j = 0; j < base_vertex_data.size(); j++) {
                int index = base_vertex_data.size() * i + j;
                vertex_data[index] = base_vertex_data[j];
                vertex_data[index].x -= 0.5 * (i % 3);
                vertex_data[index].y -= 0.5 * (i / 3);
            }
        }

        mesh_.loadVertexData(vertex_data);
    }

    program = Program::getProgram("image");
};


void Plane::render() {
    glUniformMatrix4fv(
        program->uniformLocation("model_matrix"),
        1,
        GL_FALSE,
        (GLfloat*)&model_matrix
    );
    glUniformMatrix4fv(
        program->uniformLocation("world_matrix"),
        1,
        GL_FALSE,
        (GLfloat*)&world_matrix
    );

    uint64_t vertex_start = base_vertex_data.size() * static_cast<int>(origin_);
    mesh_.render(vertex_start, 4, GL_TRIANGLE_FAN);
}
