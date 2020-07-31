#pragma once

#include <string>
#include <vector>


class Object {
public:
    Object(const std::string& name);
    Object(const Object& obj) = delete;
    ~Object();


    const std::string& name() const { return name_; }
    Object* parent() const { return parent_; }
    const std::vector<Object*>& children() const { return children_; }

    // Update own and children state recursively
    virtual void update();

    // Update self state
    virtual void updateSelf() {};

    // Add object as child and remove from previous parent if needed
    void addChild(Object& child);

    // Remove child from object and return pointer to the child
    bool removeChild(Object& child);
    bool removeChild(const std::string& child_name);

    void removeFromParent();

    // Searches children object using their instance name
    Object& getChild(const std::string& child_name);

private:
    std::string name_;
    Object* parent_;
    std::vector<Object*> children_;
};
