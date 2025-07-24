// +JMJ+
// UnifiedInputManager -- intended for use in menus, but might make a 
// suitable input manager for general use. Takes input from all possible sources. 

#pragma once

#include <unordered_set>

#include "InputManager.hpp"
#include "KeyboardManager.hpp"
#include "JoystickManager.hpp"
#include "InputManagerFactory.hpp"

namespace BT4H {

class UnifiedInputManager : public InputManager {
public:
    UnifiedInputManager(std::string appname);
    EventField getEvents(EventField* FallingEdge) override;

    SDL_GUID getGUIDOfLastUsed();
    void resetInputs();
private:
    std::vector<InputManagerPtr> _inputs;
    std::unordered_set<int> _handledDevices;
    float _getState(unsigned index);

    void _updateConnectedSticks();
    int _getDevice() { return _device; }

    std::string _appname;
    int _lastUsed;
};


}