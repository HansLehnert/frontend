#include "core/GraphicObject.h"


ObjectMap<GraphicObject> GraphicObject::object_list;
glm::mat4 GraphicObject::world_matrix(1);


GraphicObject::GraphicObject(std::string instance_name) :
    Object(instance_name)
{
	object_list_ref = object_list.insert({instance_name, this});

    visible = true;
    position = glm::vec3(0);
    scale = glm::vec3(1);

    updateModelMatrix();
}


GraphicObject::GraphicObject(GraphicObject& obj) {
	*this = obj;
}


GraphicObject::~GraphicObject() {
	object_list.erase(object_list_ref);
}


// GraphicObject& GraphicObject::operator=(GraphicObject& obj) {
// 	Object::operator=(obj);

// 	position = obj.position;
// 	scale = obj.scale;
// 	model_matrix = obj.model_matrix;

// 	program
// }


void GraphicObject::renderAll() {
	for (auto& object : object_list) {
		if (object.second->visible)
			object.second->render();
	}
}


void GraphicObject::setPosition(glm::vec3 pos) {
	position = pos;
	updateModelMatrix();
}


glm::vec3 GraphicObject::getPosition() {
	return position;
}


void GraphicObject::setScale(glm::vec3 s) {
	scale = s;
	updateModelMatrix();
}


glm::vec3 GraphicObject::getScale() {
	return scale;
}


void GraphicObject::updateModelMatrix() {
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
