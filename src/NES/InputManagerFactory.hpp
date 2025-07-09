#pragma once

#include <memory>
#include <string>
#include <cstring>
#include <SDL3/SDL.h>

#include "InputManager.hpp"
#include "KeyboardManager.hpp"
#include "JoystickManager.hpp"

namespace TC {

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

const SDL_GUID KEYBOARD = { 0 };

namespace InputManagerFactory {

std::unique_ptr<InputManager> fromGUID(SDL_GUID guid, const std::string filepath) {
    if (std::memcmp(&KEYBOARD, &guid, sizeof(SDL_GUID)) == 0) {
        return std::make_unique<KeyboardManager>(KEYBOARD_DEFAULT);
    } else {
        return NULL;
    }
}

}
}