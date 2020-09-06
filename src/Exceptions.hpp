#pragma once

#include <stdexcept>


class Exception {
public:
    const std::string& getMessage() { return message_; }

protected:
    std::string message_;
};


class ChildNotFound : public Exception {
public:
    ChildNotFound(const std::string& parent_name, const std::string& child_name) {
        message_ = "Object " + child_name + " is not a child of " + parent_name;
    }
};


class VertexIndexOutOfRange : Exception {
public:
    VertexIndexOutOfRange(uint64_t start, uint64_t count, uint64_t total_count) {
        message_ = "Vertex index out of range (start: " + std::to_string(start)
            + ", count: " + std::to_string(count)
            + ", vertex count: " + std::to_string(total_count) + ")";
    }
};
