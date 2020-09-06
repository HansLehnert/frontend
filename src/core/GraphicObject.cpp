#include "core/GraphicObject.hpp"


#include <memory>


glm::mat4 GraphicObject::world_matrix(1);


GraphicObject::GraphicObject(std::string instance_name) :
    Object(instance_name), visible_(true), position_(0), scale_(1)
{
    computeModelMatrix();
}


// GraphicObject::GraphicObject(GraphicObject& obj) {
// 	*this = obj;
// }


GraphicObject::~GraphicObject() {
}


// GraphicObject& GraphicObject::operator=(GraphicObject& obj) {
// 	Object::operator=(obj);

// 	position = obj.position;
// 	scale = obj.scale;
// 	model_matrix = obj.model_matrix;

// 	program
// }


void GraphicObject::update() {
    updateSelf();

    // Compute the transformation matrix using parent model matrix
    model_matrix_ = computeModelMatrix();

    if (parent() != nullptr) {
        model_matrix_ = parent()->model_matrix_ * model_matrix_;
    }

	if (visible_ && program_ != nullptr) {
        program_->bind();
    	render();
	}

    for (const auto& child : children()) {
        child->update();
    }
}


glm::mat4 GraphicObject::computeModelMatrix() {
	glm::mat4 matrix = glm::mat4(1);

	// Position
	matrix[3][0] = position_.x;
	matrix[3][1] = position_.y;
	matrix[3][2] = position_.z;

    // Scale
    matrix[0][0] = scale_.x;
    matrix[1][1] = scale_.y;
    matrix[2][2] = scale_.z;

	return matrix;
}
