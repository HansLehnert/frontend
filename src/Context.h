#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "core/Object.hpp"

class Context : public Object {
public:
    Context(std::string = "context");
    ~Context();

    bool init();

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

    struct InputEvent {
        enum class Type {
            KEYDOWN,
            KEYUP
        } type;

        enum class Key {
            KEY_UP,
            KEY_RIGHT,
            KEY_DOWN,
            KEY_LEFT,
            KEY_SELECT
        } key;
    };

    void dispatchEvent(InputEvent& event);
};
