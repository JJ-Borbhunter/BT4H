// +JMJ+ 
// Implementation of the UnifiedInputManager, intended for use in menus

#include "UnifiedInputManager.hpp"

/*
class UnifiedInputManager : public InputManager {
public:
    UnifiedInputManager(std::string appname) : _appname(appname) {};
    EventField getEvents(EventField* FallingEdge) override;
private:
    std::vector<InputManagerPtr> _inputs;
    std::unordered_set<int> _handledDevices;
    float _getState(unsigned index);

    void _updateConnectedSticks();

    std::string _appname;
};
*/

namespace BT4H {

UnifiedInputManager::UnifiedInputManager(std::string appname) : _appname(appname), _lastUsed(0) {
    _updateConnectedSticks();
};


EventField UnifiedInputManager::getEvents(EventField* FallingEdge = nullptr) {
    _updateConnectedSticks();

    EventField output = 0;
    EventField lastOutput = 0;
    EventField fallingEdgeInternal = 0;
    EventField* fallingEdgeInternalPtr = (FallingEdge) ? &fallingEdgeInternal : nullptr;
    for (InputManagerPtr& p : _inputs) {
        lastOutput = p->getEvents(fallingEdgeInternalPtr);
        output |= lastOutput;
        if(FallingEdge) { (*FallingEdge) |= fallingEdgeInternal; }
        if(lastOutput) { _lastUsed = p->getDevice(); }
    }
    return output;
}

float UnifiedInputManager::_getState(unsigned index) {
    float output = 0.0;
    for(InputManagerPtr& p : _inputs) {
        float curstate = p->getState(static_cast<InputButton>(1 << index));
        output = (curstate < output) ? output : curstate;
    }
    return output;
}

void UnifiedInputManager::_updateConnectedSticks() {
    if(_inputs.size() == 0) {
        _inputs.emplace_back(InputManagerFactory::fromGUID(KEYBOARD, _appname));
    }

    SDL_JoystickID* jIDs = SDL_GetJoysticks(nullptr);
    for(int i = 0; jIDs[i] != 0; i++) {
        if (_handledDevices.find(jIDs[i]) == _handledDevices.end()) { 
            auto j = InputManagerFactory::fromGUID(SDL_GetJoystickGUIDForID(jIDs[i]), _appname);
            if(j != nullptr) {
                _inputs.emplace_back(std::move(j));
            }
            _handledDevices.insert(jIDs[i]);
        }
    }
    SDL_free(jIDs);
}


void UnifiedInputManager::resetInputs() {
    _inputs.clear();
    _updateConnectedSticks();
}

// THis function exists to be used when a unified manager is created explicitly. 
// This allows the user to grab the most recently used device so they can create a manager for 
// that device and use it in their game instead of their menus.
SDL_GUID UnifiedInputManager::getGUIDOfLastUsed() {
    if(_lastUsed) {
        return KEYBOARD;
    } else {
        return SDL_GetJoystickGUIDForID(_lastUsed);
    }
}

}