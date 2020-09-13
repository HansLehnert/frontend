#pragma once

#include <string>
#include <vector>


class Object {
public:
    Object(const std::string& name);
    Object(Object&& object) noexcept;
    Object(Object& object) = delete;
    ~Object();

    const std::string& name() const { return name_; }
    Object* parent() const { return parent_; }
    const std::vector<Object*>& children() const { return children_; }

    // Update own and children state recursively
    virtual void update();

    // Add object as child and remove from previous parent if needed
    void addChild(Object& child);

    // Remove child from object
    bool removeChild(Object& child);
    bool removeChild(const std::string& child_name);

    // Searches children object using their instance name
    Object& findChild(const std::string& child_name);

protected:
    // Update self state
    virtual void updateSelf() {};

    // Called when a new child is added, can be used to modify the child
    virtual void newChild(Object& child) {};

private:
    void removeFromParent();

    std::string name_;
    Object* parent_;
    std::vector<Object*> children_;
};
