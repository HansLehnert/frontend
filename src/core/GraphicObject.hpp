#pragma once

#include "core/Object.hpp"
#include "core/Program.hpp"


class GraphicObject : public Object {
public:
    GraphicObject(std::string instance_name);
    GraphicObject(GraphicObject& obj);

    ~GraphicObject();

    //GraphicObject& operator=(GraphicObject& obj);

    virtual void step();
    virtual void render() {};

    bool visible;
    static glm::mat4 world_matrix;

    // Position
    union {
        glm::vec3 position;
        struct {
            float x;
            float y;
            float z;
        };
    };

    // Scale
    glm::vec3 scale;

protected:
    Program* program;

    glm::mat4 model_matrix;

private:
    glm::mat4 computeModelMatrix();
};
