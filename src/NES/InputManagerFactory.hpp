#pragma once

#include <memory>
#include <string>
#include <cstring>
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>
#include <vector>

#include "InputManager.hpp"
#include "KeyboardManager.hpp"
#include "JoystickManager.hpp"

namespace BT4H {

constexpr SDL_GUID KEYBOARD = { 0 };

namespace InputManagerFactory {

std::unique_ptr<InputManager> fromGUID(SDL_GUID guid, const std::string filepath);

std::unique_ptr<InputManager> initializeNew(const std::string config_filepath);

void loadAll(std::vector<std::unique_ptr<InputManager>>& vec, std::string appname);

}

}
