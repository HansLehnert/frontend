#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "core/Object.h"
#include "Message.h"

class Context : public Object {
public:
	virtual void sendMessage(Message);

	void init();
	
	int poll();
	void swapBuffers();

	int getWindowWidth();
	int getWindowHeight();
	float getAspectRatio();
private:
	static int sdl_init;

	SDL_Window* window;
	SDL_GLContext context;

	int window_width;
	int window_height;

	std::vector<Object*> input_stack;
};