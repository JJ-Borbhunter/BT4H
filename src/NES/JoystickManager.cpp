#include "JoystickManager.hpp"
#include "SaveAndLoad.hpp"
#include <cstring>

#include <bitset>

namespace BT4H {

const float DEADZONE = 0.3;
const int INT_DEADZONE = static_cast<int>(SDL_JOYSTICK_AXIS_MAX * DEADZONE);

std::string _joystick_event_name(uint8_t event) {
	switch (event) {
	case JoystickInputTypes::BUTTON: return "BUTTON";
	case JoystickInputTypes::AXIS_POSITIVE: return "AXIS+";
	case JoystickInputTypes::AXIS_NEGATIVE: return "AXIS-";
	case JoystickInputTypes::HAT_DOWN: return "HATv";
	case JoystickInputTypes::HAT_UP: return "HAT^";
	case JoystickInputTypes::HAT_LEFT: return "HAT<";
	case JoystickInputTypes::HAT_RIGHT: return "HAT>";
	default: return "N/A";
	}
	
}

JoystickManager::JoystickManager(SDL_GUID g, std::string appname) {
	SDL_JoystickID* ids = SDL_GetJoysticks(nullptr);
	for (int i = 0; ids[i] != 0; i++) {
		SDL_GUID gi = SDL_GetJoystickGUIDForID(ids[i]);
		if(std::memcmp(&g, &gi, sizeof(SDL_GUID)) == 0) {
			_device = ids[i];
			goto found;
		}
	}
	throw std::invalid_argument("Requested Joystick not connected.");
	found:
	SDL_free(ids);

	_joystick = SDL_OpenJoystick(_device);
	_binding = SaveLoad::SaveOrLoadJoystickConfig(nullptr, g, false, appname);

	// Print all binding types and indices for debugging. Uncomment the next 6 lines if needed.
	/*
	for(auto& a : _binding.types) {
		std::cout << _joystick_event_name(a) << ", ";
	}	std::cout << std::endl;
	for(auto& i : _binding.indices) {
		std::cout << (int)i << ", ";
	}	std::cout << std::endl;
	*/
}


EventField JoystickManager::getEvents(EventField* FallingEdge = nullptr) {
	EventField currentState = 0;
	EventField output;
	EventField currentBit;
	uint8_t hatState;
	for (int i = 0; i < _binding.indices.size(); ++i) {
		currentBit = 1 << i;
		switch(_binding.types[i]){
		case JoystickInputTypes::BUTTON:
			if(SDL_GetJoystickButton(_joystick, _binding.indices[i])) {
				currentState |= currentBit;
			}
			break;
		case JoystickInputTypes::AXIS_POSITIVE:
			if(SDL_GetJoystickAxis(_joystick, _binding.indices[i]) > INT_DEADZONE) {
				currentState |= currentBit;
			}
			break;
		case JoystickInputTypes::AXIS_NEGATIVE:
			if(SDL_GetJoystickAxis(_joystick, _binding.indices[i]) < -INT_DEADZONE) {
				currentState |= currentBit;
			}
			break;		
		case JoystickInputTypes::HAT_UP:
			hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
			if(hatState & SDL_HAT_UP) currentState |= currentBit;
			break;
		case JoystickInputTypes::HAT_DOWN:
			hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
			if(hatState & SDL_HAT_DOWN) currentState |= currentBit;
			break;
		case JoystickInputTypes::HAT_LEFT:
			hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
			if(hatState & SDL_HAT_LEFT) currentState |= currentBit;
			break;
		case JoystickInputTypes::HAT_RIGHT:
			hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
			if(hatState & SDL_HAT_RIGHT) currentState |= currentBit;
			break;
		}
	}

	output = currentState & ~_prevStates;
	if(FallingEdge) { *FallingEdge = ~currentState & _prevStates; }
	_prevStates = currentState;
	return output;
}

float JoystickManager::_getState(unsigned i) {

	uint8_t hatState;

	switch (_binding.types[i]) {
	    case JoystickInputTypes::BUTTON: 
	    	return (SDL_GetJoystickButton(_joystick, _binding.indices[i])) ? 1.0 : 0.0;
        case JoystickInputTypes::AXIS_POSITIVE: {
        	uint16_t state = SDL_GetJoystickAxis(_joystick, _binding.indices[i]);
        	float output = static_cast<float>(state) / SDL_JOYSTICK_AXIS_MAX;
        	if(output < DEADZONE) {
        		return 0.0;
        	}
        	return output;
        }
        case JoystickInputTypes::AXIS_NEGATIVE: {
        	uint16_t state = SDL_GetJoystickAxis(_joystick, _binding.indices[i]);
        	float output = static_cast<float>(state) / SDL_JOYSTICK_AXIS_MIN;
        	if(output < DEADZONE) {
        		return 0.0;
        	}
        	return output;
        }
        case JoystickInputTypes::HAT_UP:
        	hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
        	return (hatState & SDL_HAT_UP) ? 1.0 : 0.0;
        case JoystickInputTypes::HAT_DOWN:
        	hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
        	return (hatState & SDL_HAT_DOWN) ? 1.0 : 0.0;
        case JoystickInputTypes::HAT_LEFT:
        	hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
        	return (hatState & SDL_HAT_LEFT) ? 1.0 : 0.0;
        case JoystickInputTypes::HAT_RIGHT:
        	hatState = SDL_GetJoystickHat(_joystick, _binding.indices[i]);
        	return (hatState & SDL_HAT_RIGHT) ? 1.0 : 0.0;
	}

	return 0.0;   // just in case
}

}
