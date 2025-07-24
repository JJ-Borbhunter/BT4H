#pragma once

#include <string>
#include <SDL3/SDL_joystick.h>
#include "InputManager.hpp"

namespace BT4H {

namespace JoystickInputTypes {
const uint8_t BUTTON		= 0;
const uint8_t AXIS_POSITIVE	= 1;
const uint8_t AXIS_NEGATIVE	= 2;
const uint8_t HAT_UP		= 3;
const uint8_t HAT_DOWN		= 4;
const uint8_t HAT_LEFT		= 5;
const uint8_t HAT_RIGHT		= 6;
}

struct joystick_binding {
	std::array<uint8_t, NUM_BUTTONS_PER_BINDING> indices;
	std::array<uint8_t, NUM_BUTTONS_PER_BINDING> types;
};
typedef struct joystick_binding JoystickBinding;

class JoystickManager : public InputManager {
public:
	JoystickManager(SDL_GUID g, std::string appname);
	EventField getEvents(EventField* FallingEdge) override;

	~JoystickManager() { SDL_CloseJoystick(_joystick); }
private:
	SDL_Joystick* _joystick;
	float _getState(unsigned i) override;
	JoystickBinding _binding;
};

}
