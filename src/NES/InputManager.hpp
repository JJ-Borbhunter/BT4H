#pragma once

#include <SDL3/SDL_joystick.h>

#include <array>
#include <cstdint>
#include <memory>

namespace BT4H {

namespace Buttons {
enum buttons {
    A       = 1 << 0,
    B       = 1 << 1,
    UP      = 1 << 2,
    DOWN    = 1 << 3,
    LEFT    = 1 << 4,
    RIGHT   = 1 << 5,
    SELECT  = 1 << 6,
    START   = 1 << 7
};
}
typedef enum Buttons::buttons InputButton;

const int NUM_BUTTONS_PER_BINDING = 8;
const int KEYBOARD_ID = -1;

typedef std::array<uint16_t, NUM_BUTTONS_PER_BINDING> KeyboardBinding;
typedef uint32_t EventField;


class InputManager {
public:
    InputManager(SDL_GUID guid) : _guid(guid), _prevStates(0) {};

    float getState(InputButton b) {return _getState(_properIndex(b)); };
    virtual EventField getEvents() = 0;

    virtual ~InputManager() {}

protected:
    SDL_GUID _guid;

    EventField _prevStates;

    // Only works on GCC or CLANG. There is a c++20 option too. 
    int _properIndex(InputButton b) { return __builtin_ctz(static_cast<unsigned>(b)); }
    virtual float _getState(unsigned index) = 0;
};

typedef std::unique_ptr<InputManager> InputManagerPtr;

}