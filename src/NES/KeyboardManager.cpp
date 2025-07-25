#include <SDL3/SDL.h>
#include <bitset>
#include <iostream>

#include "KeyboardManager.hpp"
#include "SaveAndLoad.hpp"

namespace BT4H {


KeyboardManager::KeyboardManager(std::string appname) {
    _device = 0;
    _binding = SaveLoad::SaveOrLoadKeyboardConfig(nullptr, false, appname);
}

    
EventField KeyboardManager::getEvents(EventField* FallingEdge = nullptr) {
    EventField output = 0;
    EventField currentState = 0;
    uint8_t current;

    const bool* keyboardState = SDL_GetKeyboardState(nullptr);

    for (size_t i = 0; i < NUM_BUTTONS_PER_BINDING; i++) {
        current = 1 << i;
        if(keyboardState[_binding[i]]) {
            currentState |= current;
        }
    }
    
    output = currentState & ~_prevStates;
    if(FallingEdge) { *FallingEdge = ~currentState & _prevStates; }
    _prevStates = currentState;  // Save current state not current output
    return output;
}

float KeyboardManager::_getState(unsigned i) {
    const bool* keyboardState = SDL_GetKeyboardState(nullptr);
    return (keyboardState[_binding[i]]) ? 1.0 : 0.0;  // simple 1 or 0 for on or off
}

}