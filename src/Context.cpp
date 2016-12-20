#include "Context.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>

#include "Message.h"

int Context::sdl_init = 0;

void Context::init() {
	if (!sdl_init) {
		SDL_InitSubSystem(SDL_INIT_VIDEO);
		sdl_init = 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	window_width = 640;
	window_height = 480;

	window = SDL_CreateWindow("Frontend", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		std::cout << "GLEW init failed." << std::endl;

	glClearColor(0, 0, 0, 0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint null_vao;
	glGenVertexArrays(1, &null_vao);
	glBindVertexArray(null_vao);
}

int Context::poll() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
        switch (event.type) {
        	case SDL_QUIT:
        		return 0;
			case SDL_KEYDOWN: {
				Message message;
				message.type = FE_INPUT;
				message.input.source = this;
				message.input.event = FE_KEY_DOWN;
				message.input.value = event.key.keysym.sym;
				if (input_stack.size() > 0)
					input_stack.back()->sendMessage(message);
				break;
			}
			case SDL_KEYUP: {
				Message message;
				message.type = FE_INPUT;
				message.input.source = this;
				message.input.event = FE_KEY_UP;
				message.input.value = event.key.keysym.sym;
				if (input_stack.size() > 0)
					input_stack.back()->sendMessage(message);
				break;
			}
		}
    }
    return 1;
}

void Context::swapBuffers() {
	SDL_GL_SwapWindow(window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Context::sendMessage(Message message) {
	if (message.type == FE_INPUT) {
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
	}
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