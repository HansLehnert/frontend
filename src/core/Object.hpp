#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"


class Object : public std::enable_shared_from_this<Object> {
public:
    Object(std::string name);
    Object(const Object& obj);
    ~Object();

    Object& operator=(const Object& obj);

    virtual void step();
    virtual void update() {};

    void addChild(std::shared_ptr<Object> child);
    void removeChild(std::shared_ptr<Object> child);
    void removeChild(std::string child_name);
    std::shared_ptr<Object> getChild(std::string child_name);

protected:
    std::string instance_name;

    std::weak_ptr<Object> parent;
    std::vector<std::shared_ptr<Object>> children;
};
