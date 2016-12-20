#include "core/Object.h"

#include <list>
#include "glm/glm.hpp"
#include "Message.h"

std::list<Object*> Object::object_list;
glm::mat4 Object::world_matrix(1);

Object::Object() : model_matrix(1) {
	object_list.push_front(this);
	list_pos = object_list.begin();
}

Object::~Object() {
	object_list.erase(list_pos);
}

void Object::updateAll() {
	for (std::list<Object*>::iterator i = object_list.begin(); i != object_list.end(); i++) {
		(*i)->update();
	}
}

void Object::renderAll() {
	for (std::list<Object*>::iterator i = object_list.begin(); i != object_list.end(); i++) {
		(*i)->render();
	}
}

void Object::sendMessageAll(Message message) {
	for (std::list<Object*>::iterator i = object_list.begin(); i != object_list.end(); i++) {
		(*i)->sendMessage(message);
	}
}