#include "InputManagerFactory.hpp"

namespace BT4H {

namespace InputManagerFactory {
    

std::unique_ptr<InputManager> fromFile(std::string filename) {
    return std::make_unique<KeyboardManager>(KEYBOARD_DEFAULT);
}

}

}