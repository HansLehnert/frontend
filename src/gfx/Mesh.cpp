#include "Mesh.hpp"

#include "gl_inc.h"


Mesh::~Mesh() {
    if (model_buffer_ != 0)
        glDeleteBuffers(1, &model_buffer_);
    if (index_buffer_ != 0)
        glDeleteBuffers(1, &index_buffer_);
}


void Mesh::render(int64_t start_index, uint64_t count, GLuint draw_mode) const {
    glBindBuffer(GL_ARRAY_BUFFER, model_buffer_);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    glDrawArrays(draw_mode, start_index, count);
}


size_t Mesh::loadData(GLuint& buffer, const Vertex* data, size_t count) {
    size_t total_size = count * sizeof(Vertex);

    if (buffer == 0)
        glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, total_size, data, GL_STATIC_DRAW);

    return total_size;
}
