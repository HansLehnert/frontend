#include "Context.h"

#include <iostream>

#include "gl_inc.h"

#ifdef RASPBERRY_PI
#include <SDL2/SDL_opengles.h>
#else
#include <SDL2/SDL_opengl.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "core/Event.h"


int Context::sdl_init = 0;


Context::Context(std::string instance_name) : Object(instance_name) {
	
}


void Context::init() {
	//SDL initialization
	if (!sdl_init) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 0) {
			sdl_init = 1;
			std::cout << "[Context]\tInitialized SDL" << std::endl;
		}
		else {
			std::cout << "[Context]\tFailed to initialize SDL" << std::endl;
		}


		if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)) {
			std::cout << "[Context]\tFailed to initialize image libraries" << std::endl;
		}
	}

	//GL context configuration

#ifdef RASPBERRY_PI
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
#endif

	//Window size
	window_width = 640;
	window_height = 480;

	//Window and context creation
	window = SDL_CreateWindow("Frontend", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL);
	if ((context = SDL_GL_CreateContext(window)) == 0) {
		std::cout << "[Context]\tFailed to create context" << std::endl;
	}


#ifndef RASPBERRY_PI
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		std::cout << "[Context]\tGLEW init failed." << std::endl;
#endif

	//GL configuration
	glClearColor(0, 0, 0, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef RASPBERRY_PI
	GLuint null_vao;
	glGenVertexArrays(1, &null_vao);
	glBindVertexArray(null_vao);
#endif
}

int Context::poll() {
	SDL_Event sdl_event;
	while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
        	case SDL_QUIT:
        		return 0;

        	//Handle keyboard input
			case SDL_KEYDOWN: {
				Event key_event;
				key_event.type = EVENT_INPUT_KEYDOWN;

				switch (sdl_event.key.keysym.sym) {
					case SDLK_UP:
						key_event.input.key = KEY_UP;
						break;
					case SDLK_DOWN:
						key_event.input.key = KEY_DOWN;
						break;
					case SDLK_LEFT:
						key_event.input.key = KEY_LEFT;
						break;
					case SDLK_RIGHT:
						key_event.input.key = KEY_RIGHT;
						break;
					case SDLK_RETURN:
						key_event.input.key = KEY_SELECT;
						break;
					default:
						return 1;
				}

				dispatchEvent(key_event);
				/*message.input.value = event.key.keysym.sym;
				if (input_stack.size() > 0)
					input_stack.back()->sendMessage(message);*/
				break;
			}
			/*case SDL_KEYUP: {
				Message message;
				message.type = FE_INPUT;
				message.input.source = this;
				message.input.event = FE_KEY_UP;
				message.input.value = event.key.keysym.sym;
				if (input_stack.size() > 0)
					input_stack.back()->sendMessage(message);
				break;
			}*/
		}
		continue;
    }
    return 1;
}


void Context::pause() {
#ifdef RASPBERRY_PI
	SDL_GL_DeleteContext(context);
#endif
}


void Context::resume() {
#ifdef RASPBERRY_PI
	context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, context);
#endif
}


void Context::swapBuffers() {
	SDL_GL_SwapWindow(window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Context::handleEvent(Event& event) {
	/*if (message.type == FE_INPUT) {
		switch (message.input.event) {
			case FE_START_LISTEN: {
				if (message.common.source != NULL)
					input_stack.push_back(message.input.source);
				break;
			}
			case FE_STOP_LISTEN: {
				for (auto i = input_stack.begin(); i != input_stack.end(); i++) {
					if (*i == message.common.source) {
						input_stack.erase(i);
					}
				}
				break;
			}
			default:
				break;
		}
	}*/
}


int Context::getWindowWidth() {
	return window_width;
}


int Context::getWindowHeight() {
	return window_height;
}


float Context::getAspectRatio() {
	return window_width / (float) window_height;
}
