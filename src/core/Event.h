/*
Event system for communication between objects
*/

#pragma once

enum EventType {
	//Keyboard Events
	EVENT_INPUT_KEYDOWN,
	EVENT_INPUT_KEYUP,

	EVENT_NULL
};

//Input event enums

enum Key {
	KEY_UP,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_SELECT
};


//Event structure

class Object; //Forward declaration to use pointer

struct Event {
	EventType type;
	Object* source;

	union {
		//Input Events
		struct {
			Key key;
		} input;
	};
};