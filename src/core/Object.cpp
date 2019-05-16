#include "core/Object.hpp"

#include <algorithm>

#include <list>
#include "glm/glm.hpp"


Object::Object(std::string instance_name) {
    this->instance_name = instance_name;
    parent = nullptr;
}


Object::Object(const Object& obj) {
    instance_name = obj.instance_name + "_copy";
}


Object& Object::operator=(const Object& obj) {
    (*this) = obj;

    return *this;
}


Object::~Object() {
    for (std::shared_ptr<Object>& child : children) {
        child->parent = nullptr;
    }
}


void Object::step() {
    update();

    for (std::shared_ptr<Object>& child : children) {
        child->step();
    }
}


void Object::addChild(std::shared_ptr<Object> child) {
    // Check whether the object is already a child
    for (std::shared_ptr<Object>& c : children) {
        if (child == c) {
            return;
        }
    }

    // Remove old parent from child
    if (child->parent != nullptr) {
        child->parent->removeChild(child);
    }

    children.push_back(child);
    child->parent = this;
}


void Object::removeChild(std::shared_ptr<Object> child) {
    for (auto i = children.begin(); i != children.end(); i++) {
        if (*i == child) {
            child->parent = nullptr;
            children.erase(i);
            break;
        }
    }
}


void Object::removeChild(std::string child_name) {
    for (auto i = children.begin(); i != children.end(); i++) {
        if ((*i)->instance_name == child_name) {
            (*i)->parent = nullptr;
            children.erase(i);
            break;
        }
    }
}


std::shared_ptr<Object> Object::getChild(std::string child_name) {
    for (std::shared_ptr<Object>& child : children) {
        if (child->instance_name == child_name) {
            return child;
        }
    }
}
