#include "Mesh.hpp"

#include <type_traits>


template<typename T>
void Mesh::loadVertexData(const T& data) {
    static_assert(std::is_same<typename T::value_type, Vertex>::value);
    model_buffer_size_ = loadData(model_buffer_, data.data(), data.size());
    vertex_count_ = data.size();
}
