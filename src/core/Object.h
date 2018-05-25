#pragma once

#include <string>
#include <vector>
#include <list>
#include <functional>
#include <unordered_map>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"

#include "core/ObjectMap.h"
#include "core/Event.h"


class Object {
public:
	Object(std::string name = "");
	Object(const Object& obj);
	~Object();

	Object& operator=(const Object& obj);

	virtual void update() {};
	static void updateAll();

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

private:
	std::string name;

	static ObjectMap<Object> object_list;
	ObjectMapRef<Object> object_list_ref;

	//typedef std::pair<Object*, int> Listener;

	static std::vector<std::list<Listener> > global_listeners;
	std::vector<std::list<Listener> > local_listeners;
};
