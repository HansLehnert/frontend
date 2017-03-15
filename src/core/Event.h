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

class Object; //Forward declaration to use pointer

struct Event {
	EventType type;
	Object* source;

	union {
		//Input Events
		struct {
			int keycode;
			int keychar;
		} input;
	};
};