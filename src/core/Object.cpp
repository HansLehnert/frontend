#include "core/Object.hpp"

#include <algorithm>
#include "Exceptions.hpp"


Object::Object(const std::string& instance_name) :
        name_(instance_name),
        parent_(nullptr) {
}


Object::~Object() {
    for (auto& child : children_) {
        child->parent_ = nullptr;
    }
}


void Object::update() {
    updateSelf();

    for (auto& child : children_) {
        child->update();
    }
}


void Object::addChild(Object& child) {
    child.removeFromParent();

    children_.push_back(&child);
    child.parent_ = this;
}


bool Object::removeChild(Object& child) {
    if (child.parent_ != this)
        return false;

    for (auto i = children_.begin(); i != children_.end(); i++) {
        if (*i == &child) {
            child.parent_ = nullptr;
            children_.erase(i);
            return true;
        }
    }
}


bool Object::removeChild(const std::string& child_name) {
    for (auto i = children_.begin(); i != children_.end(); i++) {
        Object& child = **i;

        if (child.name_ == child_name) {
            child.parent_ = nullptr;
            children_.erase(i);
            return true;
        }
    }

    return false;
}


void Object::removeFromParent() {
    if (parent_ == nullptr)
        return;

    if (!parent_->removeChild(*this))
        throw ChildNotFound(parent_->name_, name_);
}


Object& Object::getChild(const std::string& child_name) {
    for (auto& child : children_) {
        if (child->name_ == child_name) {
            return *child;
        }
    }

    throw ChildNotFound(name_, child_name);
}
