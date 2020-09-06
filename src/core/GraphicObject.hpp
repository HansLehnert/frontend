#pragma once

#include "glm/glm.hpp"
#include "core/Object.hpp"
#include "core/Program.hpp"


class GraphicObject : public Object {
public:
    // Update and render self and child objects
    void update() override final;

protected:
    GraphicObject(std::string instance_name);
    GraphicObject(GraphicObject&&) = default;
    ~GraphicObject();

    // Used to implement the actual drawing routine
    virtual void render() const = 0;

    Program& program() const { return *program_; } // TODO: should also be set with setter

    const GraphicObject* parent() const {
        return dynamic_cast<const GraphicObject*>(Object::parent());
    }

public:
    static glm::mat4 world_matrix;

    bool visible_;
    glm::vec3 position_;
    glm::vec3 scale_;

protected:
    glm::mat4 computeModelMatrix();
    glm::mat4 model_matrix_;

    Program* program_;
};
