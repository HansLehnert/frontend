#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "core/Object.h"
#include "core/Event.h"

class Context : public Object {
public:
	Context(std::string = "context");
	~Context();

	virtual void handleEvent(Event&);

	void init();

	void pause();
	void resume();
	
	int poll();
	void swapBuffers();

	int getWindowWidth();
	int getWindowHeight();
	float getAspectRatio();

	static int eventFilter(void*, SDL_Event*);
private:
	static int sdl_init;

	int running;

	SDL_Window* window;
	SDL_GLContext context;

	int window_width;
	int window_height;

	std::vector<SDL_Joystick*> joystick_id;
	std::vector<int> joystick_axis;
	enum JoystickPosition {
		JOYSTICKPOS_UP,
		JOYSTICKPOS_DOWN,
		JOYSTICKPOS_NONE
	} joystick_pos[2];
	int joystick_repeat_timer;
};