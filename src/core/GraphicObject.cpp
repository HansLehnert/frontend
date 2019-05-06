#include "core/GraphicObject.hpp"


glm::mat4 GraphicObject::world_matrix(1);


GraphicObject::GraphicObject(std::string instance_name) :
    Object(instance_name)
{
    visible = true;
    position = glm::vec3(0);
    scale = glm::vec3(1);

    computeModelMatrix();
}


// GraphicObject::GraphicObject(GraphicObject& obj) {
// 	*this = obj;
//}


GraphicObject::~GraphicObject() {
}


// GraphicObject& GraphicObject::operator=(GraphicObject& obj) {
// 	Object::operator=(obj);

// 	position = obj.position;
// 	scale = obj.scale;
// 	model_matrix = obj.model_matrix;

// 	program
// }


void GraphicObject::step() {
    update();

    // Compute the transformation matrix using parent model matrix
    model_matrix = computeModelMatrix();

    if (!parent.expired()) {
        std::shared_ptr<GraphicObject> graphic_parent =
            std::dynamic_pointer_cast<GraphicObject>(parent.lock());

        if (graphic_parent) {
            model_matrix = graphic_parent->model_matrix * model_matrix;
        }
    }

	if (visible) {
    	render();
	}

    for (int i = 0; i < MAX_CHILDREN; i++) {
        if (children[i] != nullptr) {
            children[i]->step();
        }
    }
}


glm::mat4 GraphicObject::computeModelMatrix() {
	glm::mat4 matrix = glm::mat4(1);

	// Position
	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2] = z;

    // Scale
    matrix[0][0] = scale.x;
    matrix[1][1] = scale.y;
    matrix[2][2] = scale.z;

	return matrix;
}
