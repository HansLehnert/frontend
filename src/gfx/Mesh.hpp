#pragma once

#include "gl_inc.h"


struct Vertex {
    float x, y, z, w, u, v;
};


// Manages a GL Buffer for storing vertex buffer
class Mesh {
    friend class Plane;

public:
    Mesh() = default;
    ~Mesh();

    bool hasVertexData() const { return model_buffer_ != 0; }

    // Load data from a std::vector / std::array
    template<typename T>
    void loadVertexData(const T& data);

    void render(uint64_t start_index, uint64_t count, GLuint draw_mode) const;

private:
    size_t loadData(GLuint& buffer, const Vertex* data, size_t count);

private:
    GLuint model_buffer_;
    GLuint index_buffer_;

    size_t model_buffer_size_;
    size_t index_buffer_size_;

    uint64_t vertex_count_;

};


#include "Mesh.inl"
