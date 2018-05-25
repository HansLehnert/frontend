#include "core/Object.h"

#include <iostream>
#include <algorithm>

#include <list>
#include "glm/glm.hpp"


ObjectMap<Object> Object::object_list;


Object::Object(std::string instance_name) :
		name(instance_name),
		local_listeners(EVENT_NULL)
{
	object_list_ref = object_list.insert({name, this});
}


Object::Object(const Object& obj) {
	name = obj.name + "_copy";
}


Object& Object::operator=(const Object& obj) {
	(*this) = obj;

	return *this;
}


Object::~Object() {
	object_list.erase(object_list_ref);
}


void Object::updateAll() {
	for (auto& object : object_list) {
		object.second->update();
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
