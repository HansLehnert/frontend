#pragma once

#include "core/Object.h"
#include "core/Program.h"

class GraphicObject : public Object {
public:
    GraphicObject(std::string instance_name);
    GraphicObject(GraphicObject& obj);

	~GraphicObject();

	//GraphicObject& operator=(GraphicObject& obj);

	virtual void render() {};
	static void renderAll();

	bool visible;
	static glm::mat4 world_matrix;

	void setPosition(glm::vec3 new_position);
	glm::vec3 getPosition();

	void setScale(glm::vec3 new_scale);
	glm::vec3 getScale();

private:
	static ObjectMap<GraphicObject> object_list;
	ObjectMapRef<GraphicObject> object_list_ref;

protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 model_matrix;

	Program* program;

	void updateModelMatrix();
};
