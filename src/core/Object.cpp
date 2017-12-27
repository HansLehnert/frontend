#include "core/Object.h"

#include <iostream>
#include <algorithm>

#include <list>
#include "glm/glm.hpp"


std::unordered_multimap<std::string, Object*> Object::object_list;
glm::mat4 Object::world_matrix(1);


Object::Object(std::string instance_name) :
	name(instance_name),
	local_listeners(EVENT_NULL),
	visible(true)
{
	position = glm::vec3(0);
	scale = glm::vec3(1);
	computeModelMatrix();

	list_pos = object_list.insert(std::pair<std::string, Object*>(name, this));
}


Object::~Object() {
	object_list.erase(list_pos);
}


void Object::updateAll() {
	for (auto& object : object_list) {
		object.second->update();
	}
}


void Object::renderAll() {
	for (auto& object : object_list) {
		if (object.second->visible)
			object.second->render();
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

//////////////////////////////////////////////////////////
//Graphic methods
//////////////////////////////////////////////////////////

void Object::setPosition(glm::vec3 pos) {
	position = pos;
	computeModelMatrix();
}


glm::vec3 Object::getPosition() {
	return position;
}


void Object::setScale(glm::vec3 s) {
	scale = s;
	computeModelMatrix();
}


glm::vec3 Object::getScale() {
	return scale;
}


void Object::computeModelMatrix() {
	model_matrix = glm::mat4(1);

	//Position
	model_matrix[3][0] = position.x;
	model_matrix[3][1] = position.y;
	model_matrix[3][2] = position.z;

	//Scale
	model_matrix[0][0] = scale.x;
	model_matrix[1][1] = scale.y;
	model_matrix[2][2] = scale.z;
}