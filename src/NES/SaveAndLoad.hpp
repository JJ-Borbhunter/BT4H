
#include <iostream>
#include <fstream>
#include <cstring>

#include "InputManager.hpp"
#include "KeyboardManager.hpp"
#include "JoystickManager.hpp"

namespace BT4H {

namespace SaveLoad {

constexpr bool SAVE = true;
constexpr bool LOAD = false;

typedef std::array<uint8_t, NUM_BUTTONS_PER_BINDING * 2> ConfigBinary;

KeyboardBinding SaveOrLoadKeyboardConfig(KeyboardBinding* k, bool save, std::string saveDir);

JoystickBinding SaveOrLoadJoystickConfig(JoystickBinding* j, SDL_GUID guid, bool save, std::string saveDir);

}


}