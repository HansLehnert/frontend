#pragma once

#include <list>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "Message.h"

class Object {
public:
	Object();
	~Object();

	virtual void update() {};
	virtual void render() {};
	virtual void sendMessage(Message) {};

	static void updateAll();
	static void renderAll();
	static void sendMessageAll(Message);

	static glm::mat4 world_matrix;
	glm::mat4 model_matrix;
	int visible;
private:
	static std::list<Object*> object_list;
	std::list<Object*>::iterator list_pos;
};
