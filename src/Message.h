#pragma once

#include <SDL2/SDL_events.h>

class Object;

enum MessageType {
	FE_INPUT
};

struct CommonMessage {
	int type;
	Object* source;
};

enum InputEvent {
	FE_START_LISTEN,
	FE_STOP_LISTEN,
	FE_KEY_DOWN,
	FE_KEY_UP
};

struct InputMessage : public CommonMessage {
	int event;
	int value;
};

union Message {
	int type;
	CommonMessage common;
	InputMessage input;
};