#pragma once

#include <vector>
#include <list>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"

#include "Event.h"

class Object {
public:
	Object();
	~Object();

	virtual void update() {};
	static void updateAll();

	virtual void render() {};
	static void renderAll();

	void addListener(EventType);
	void removeListener(EventType);
	void dispatchEvent(Event&);
	virtual void handleEvent(Event&) {};

	static glm::mat4 world_matrix;

	///////////////////////////////////////////////////////////////////////////
	//Graphic members
	///////////////////////////////////////////////////////////////////////////
	bool visible;

	void setPosition(glm::vec3);
	glm::vec3 getPosition();

	void setScale(glm::vec3);
	glm::vec3 getScale();

protected:

	///////////////////////////////////////////////////////////////////////////
	//Graphic members
	///////////////////////////////////////////////////////////////////////////

	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 model_matrix;

	void computeModelMatrix();

private:
	static std::list<Object*> object_list;
	std::list<Object*>::iterator list_pos; //Stores position in list

	typedef std::pair<Object*, int> Listener;
	static std::vector<std::list<Object*> > listener;
};
