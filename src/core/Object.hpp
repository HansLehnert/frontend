#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"

#include "core/ObjectMap.h"
#include "core/Event.h"


class Object : public std::enable_shared_from_this<Object> {
public:
    Object(std::string name = "");
    Object(const Object& obj);
    ~Object();

    Object& operator=(const Object& obj);

    virtual void step();
    virtual void update() {};

    void addListener(
        EventType event_type,
        Callback callback,
        Object* source = nullptr,
        ListenerPriority priority = PRIORITY_EQUAL);

    void removeListener(
        EventType event_type,
        Object* source = nullptr);

    void removeListener(Listener listener);

    void dispatchEvent(Event& event);

    void addChild(std::shared_ptr<Object> child);
    void removeChild(std::shared_ptr<Object> child);
    void removeChild(std::string child_name);
    std::shared_ptr<Object> getChild(std::string child_name);

protected:
    std::string instance_name;

    std::weak_ptr<Object> parent;
    std::vector<std::shared_ptr<Object>> children;

private:
    static std::vector<std::list<Listener> > global_listeners;
    std::vector<std::list<Listener> > local_listeners;
};
