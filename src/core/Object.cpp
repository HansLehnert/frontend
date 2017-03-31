#include "core/Object.h"

#include <iostream>

#include <list>
#include "glm/glm.hpp"

std::unordered_multimap<std::string, Object*> Object::object_list;
glm::mat4 Object::world_matrix(1);

Object::Object(std::string instance_name) : model_matrix(1) {
	name = instance_name;
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
		object.second->render();
	}
}


/*void Object::sendMessageAll(Message message) {
	for (std::list<Object*>::iterator i = object_list.begin(); i != object_list.end(); i++) {
		(*i)->sendMessage(message);
	}
}*/

///////////////////////////////////////////////////////////////////////////////
//Event System
///////////////////////////////////////////////////////////////////////////////

std::vector<std::list<Object*> > Object::listener(EVENT_NULL);

void Object::addListener(EventType type) {
	listener[type].push_front(this);
}


void Object::removeListener(EventType type) {
	listener[type].remove(this);
}


void Object::dispatchEvent(Event& event) {
	EventType type = event.type;

	event.source = this;

	for (auto object : listener[type]) {
		object->handleEvent(event);
	}
}

///////////////////////////////////////////////////////////////////////////////
//Graphic methods
///////////////////////////////////////////////////////////////////////////////

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