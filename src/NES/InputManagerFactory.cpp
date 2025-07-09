#include "InputManagerFactory.hpp"

namespace TC {

namespace InputManagerFactory {
    

std::unique_ptr<InputManager> fromFile(std::string filename) {
    return std::make_unique<KeyboardManager>(KEYBOARD_DEFAULT);
}

}

}