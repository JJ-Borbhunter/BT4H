#pragma once
#include "InputManager.hpp"

namespace BT4H {
    
typedef std::array<uint16_t, NUM_BUTTONS_PER_BINDING> KeyboardBinding;

class KeyboardManager : public InputManager {
public:
    KeyboardManager(KeyboardBinding b) : InputManager({0}), _binding(b) {};

    EventField getEvents() override;
private:
    KeyboardBinding _binding;
    float _getState(unsigned i) override;
};

}
