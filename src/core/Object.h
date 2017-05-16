#pragma once

#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include <SDL2/SDL.h>
#include "glm/glm.hpp"

#include "Event.h"
#include "Program.h"

class Object {
public:
	Object(std::string);
	~Object();

	virtual void update() {};
	static void updateAll();

	virtual void render() {};
	static void renderAll();

private:
	std::string name;
	static std::unordered_multimap<std::string, Object*> object_list;
	std::unordered_multimap<std::string, Object*>::iterator list_pos; //Stores position in list

	//////////////////////////////////////////////////////////
	//Event system
	//////////////////////////////////////////////////////////
public:
	void addListener(EventType, Object* = nullptr);
	void removeListener(EventType, Object* = nullptr);
	void dispatchEvent(Event&);
	virtual void handleEvent(Event&) {};
	
private:
	typedef std::pair<Object*, int> Listener;

	static std::vector<std::list<Object*> > global_listener;
	std::vector<std::list<Object*> > local_listener;

	//////////////////////////////////////////////////////////
	//Graphic members
	//////////////////////////////////////////////////////////
public:
	bool visible;
	static glm::mat4 world_matrix;

	void setPosition(glm::vec3);
	glm::vec3 getPosition();

	void setScale(glm::vec3);
	glm::vec3 getScale();

protected:
	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 model_matrix;

	Program* program;

	void computeModelMatrix();
};
