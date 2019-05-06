#include "core/Object.hpp"

#include <iostream>
#include <algorithm>

#include <list>
#include "glm/glm.hpp"


Object::Object(std::string instance_name) : local_listeners(EVENT_NULL) {
    this->instance_name = instance_name;
}


Object::Object(const Object& obj) {
    instance_name = obj.instance_name + "_copy";
}


Object& Object::operator=(const Object& obj) {
    (*this) = obj;

    return *this;
}


Object::~Object() {
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (children[i] != nullptr) {
            children[i]->parent = std::weak_ptr<Object>();
        }
    }

    std::cout << "Destroyed " << instance_name << std::endl;
}


void Object::step() {
    update();

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (children[i] != nullptr) {
            children[i]->step();
        }
    }
}


/*void Object::sendMessageAll(Message message) {
    for (std::list<Object*>::iterator i = object_list.begin(); i != object_list.end(); i++) {
        (*i)->sendMessage(message);
    }
}*/

//////////////////////////////////////////////////////////
//Event System
//////////////////////////////////////////////////////////

std::vector<std::list<Listener> > Object::global_listeners(EVENT_NULL);

void Object::addListener(EventType type, Callback callback, Object* source, ListenerPriority priority) {
    Listener listener;
    listener.target = this;
    listener.priority = priority;
    listener.callback = callback;

    if (source == nullptr)
        global_listeners[type].push_front(listener);
    else
        source->local_listeners[type].push_front(listener);
}


void Object::removeListener(EventType type, Object* source) {
    if (source == nullptr) {
        global_listeners[type].erase(
            std::remove_if(
                global_listeners[type].begin(),
                global_listeners[type].end(),
                [this] (Listener& l) { return l.target == this; }),
            global_listeners[type].end());
    }
    else {
        source->local_listeners[type].erase(
            std::remove_if(
                source->local_listeners[type].begin(),
                source->local_listeners[type].end(),
                [this] (Listener& l) { return l.target == this; }),
            source->local_listeners[type].end());
    }
}


void Object::dispatchEvent(Event& event) {
    EventType type = event.type;
    event.source = this;

    for (auto& listener : global_listeners[type]) {
        listener.callback(event);
        if (listener.priority == PRIORITY_TOP)
            break;
    }

    for (auto& listener : local_listeners[type]) {
        listener.callback(event);
        if (listener.priority == PRIORITY_TOP)
            break;
    }
}


void Object::addChild(std::shared_ptr<Object> child) {
    // Search an empty slot
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (children[i] == nullptr) {
            // Remove old parent from child
            std::shared_ptr<Object> old_parent = child->parent.lock();
            if (old_parent) {
                old_parent->removeChild(child);
            }

            children[i] = child;
            child->parent = std::weak_ptr<Object>(shared_from_this());
            break;
        }
        if (children[i] == child) {
            break;
        }
    }
}


void Object::removeChild(std::shared_ptr<Object> child) {
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (children[i] == child) {
            child->parent = std::weak_ptr<Object>();
            children[i] = nullptr;
            break;
        }
    }
}


void Object::removeChild(std::string child_name) {
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (children[i]->instance_name == child_name) {
            children[i]->parent = std::weak_ptr<Object>();
            children[i] = nullptr;
            break;
        }
    }
}


std::shared_ptr<Object> Object::getChild(std::string child_name) {
    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (children[i]->instance_name == child_name) {
            return children[i];
        }
    }
}
