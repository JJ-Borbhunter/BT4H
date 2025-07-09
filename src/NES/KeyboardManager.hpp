#include "InputManager.hpp"

namespace TC {

class KeyboardManager : public InputManager {
public:
    KeyboardManager(KeyboardBinding b) : InputManager({0}), _binding(b) {};

    EventField getEvents() override;
private:
    KeyboardBinding _binding;
    float _getState(unsigned i) override;
};

}
