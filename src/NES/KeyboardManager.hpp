#pragma once
#include <SDL3/SDL.h>
#include "InputManager.hpp"

namespace BT4H {
    
typedef std::array<uint16_t, NUM_BUTTONS_PER_BINDING> KeyboardBinding;

const KeyboardBinding KEYBOARD_DEFAULT {
    SDL_SCANCODE_X,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_RIGHT,
    SDL_SCANCODE_RETURN,
    SDL_SCANCODE_ESCAPE
};

class KeyboardManager : public InputManager {
public:
    KeyboardManager(std::string appname);

    EventField getEvents() override;
private:
    KeyboardBinding _binding;
    float _getState(unsigned i) override;
};

}
