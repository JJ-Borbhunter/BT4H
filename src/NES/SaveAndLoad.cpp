
#include <iostream>
#include <fstream>
#include <cstring>
#include <SDL3/SDL_joystick.h>
#include <SDL3/SDL_filesystem.h>
#include "SaveAndLoad.hpp"

namespace BT4H {

namespace SaveLoad {

ConfigBinary _save_or_load_config(ConfigBinary* cb, SDL_GUID guid, bool save, std::string saveDir) {
    char guidCStr[64];
    SDL_GUIDToString(guid, guidCStr, 64);
    std::string guidStr(guidCStr);
    
    char* saveDirCStr = SDL_GetPrefPath("", saveDir.c_str());
    std::string savefile(saveDirCStr);
    SDL_free(saveDirCStr);

    savefile += guidStr;
    std::cout << savefile << std::endl;

    ConfigBinary b = {0};

    if(save) {
        std::ofstream outFile(savefile, std::ios::binary | std::ios::out);
        if(!outFile) throw std::ios_base::failure("Failed to open Joystick config file");
        outFile.write(reinterpret_cast<char*>(cb), (*cb).size());
        outFile.close();
    } else {
        std::ifstream infile(savefile, std::ios::binary | std::ios::in);
        if(infile) {
            infile.read(reinterpret_cast<char*>(&b), b.size());
            infile.close();
        } else {
            throw std::invalid_argument("File for device GUID not found.");
        }
    }

    return b;
}

KeyboardBinding SaveOrLoadKeyboardConfig(KeyboardBinding* k, bool save, std::string saveDir) {
    try {
        ConfigBinary cb = _save_or_load_config(reinterpret_cast<ConfigBinary*>(k), {0}, save, saveDir);
        KeyboardBinding k_return;
        std::memcpy(&k_return, &cb, cb.size());
        return k_return;
    } catch(std::invalid_argument) {
        return KEYBOARD_DEFAULT;
    }
}

JoystickBinding SaveOrLoadJoystickConfig(JoystickBinding* j, SDL_GUID guid, bool save, std::string saveDir) {
    ConfigBinary cb = _save_or_load_config(reinterpret_cast<ConfigBinary*>(j), guid, save, saveDir);
    JoystickBinding j_return;
    std::memcpy(&j_return, &cb, cb.size());
    return j_return;
}

}


}