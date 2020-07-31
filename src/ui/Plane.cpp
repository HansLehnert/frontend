#include "Plane.hpp"

#include <array>

#include "glm/glm.hpp"
#include "core/Texture.hpp"
#include "core/Program.hpp"


// Model data
struct Vertex {
    float x, y, z, w, u, v;
};

const std::array<Vertex, 5> base_vertex_data = {
    0, 0, 0, 1, 0, 0,
    1, 0, 0, 1, 1, 0,
    1, 1, 0, 1, 1, 1,
    0, 1, 0, 1, 0, 1,
    0, 0, 0, 1, 0, 0,
};


GLuint Plane::model_buffer = 0;


Plane::Plane(std::string instance_name) :
        GraphicObject(instance_name),
        origin(Plane::Origin::TOP_LEFT)
{
    // Initialize the quad model buffer for rendering the image
    if (model_buffer == 0) {
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

        glGenBuffers(1, &model_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(vertex_data),
            vertex_data.data(),
            GL_STATIC_DRAW
        );
    }

    program = Program::getProgram("image");
};


void Plane::render() {
    glBindBuffer(GL_ARRAY_BUFFER, model_buffer);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));

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

    glDrawArrays(GL_TRIANGLE_FAN, base_vertex_data.size() * (int)origin, 4);
}
