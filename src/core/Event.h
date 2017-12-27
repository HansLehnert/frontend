/*
Event system definitions
*/

#pragma once


enum ListenerPriority {
	PRIORITY_EQUAL,
	PRIORITY_TOP
};


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
	//std::list<Listener>::iterator listener;

	union {
		//Input Events
		struct {
			Key key;
		} input;
	};
};


typedef std::function<void(Event)> Callback;


struct Listener {
	Object* target;
	ListenerPriority priority;
	Callback callback;
};