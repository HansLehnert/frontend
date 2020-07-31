#pragma once

#include "glm/glm.hpp"
#include "core/Object.hpp"
#include "core/Program.hpp"


class GraphicObject : public Object {
public:
    GraphicObject(std::string instance_name);
    GraphicObject(GraphicObject& obj) = delete;

    ~GraphicObject();


    void update() override final;
    virtual void render() = 0;

    const GraphicObject* parent() const { return dynamic_cast<const GraphicObject*>(Object::parent()); }

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
    glm::mat4 computeModelMatrix();

    Program* program;

    glm::mat4 model_matrix;
};
