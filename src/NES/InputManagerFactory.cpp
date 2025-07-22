// +JMJ +
// Factory pattern header for input managers

#include "InputManagerFactory.hpp"
#include "image.h"
#include "SaveAndLoad.hpp"


#include <iostream>
#include <vector>


constexpr int BMP_CHAR_HEIGHT = 9;
constexpr int BMP_CHAR_WIDTH = 7;
constexpr int BMP_CHAR_ROW_LEN = 18;
constexpr int FONT_HEIGHT = 16;
constexpr int FONT_WIDTH = 12;

constexpr float DEADZONE = 0.95;

namespace BT4H {

namespace InputManagerFactory {

void _drawText(const char* text, int x, int y, SDL_Renderer* r, SDL_Texture* font);
int _newline(int i);

std::unique_ptr<InputManager> fromGUID(SDL_GUID guid, const std::string appname) {
    if (std::memcmp(&KEYBOARD, &guid, sizeof(SDL_GUID)) == 0) {
        return std::make_unique<KeyboardManager>(appname);
    } else {
        try {
            std::cout << "Trying joystick" << std::endl;
            return std::make_unique<JoystickManager>(guid, appname);
        } catch (std::invalid_argument) {
            return nullptr;
        }
    }
}


SDL_Texture* _SDL_TexFromMemoryConstant(unsigned char* data, int len, SDL_Renderer* r) {
    SDL_IOStream* IO = SDL_IOFromMem(data, len);

    SDL_Surface* surface = SDL_LoadBMP_IO(IO, true);
    if (!surface) {
        SDL_Log("LoadBMP failed: %s", SDL_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(r, surface);
    if (!texture) {
        SDL_Log("CreateTextureFromSurface failed: %s", SDL_GetError());
    }

    SDL_DestroySurface(surface); // you don’t need the surface anymore
    return texture;
}

SDL_Texture* _LoadFontTex(SDL_Renderer* r){
    return _SDL_TexFromMemoryConstant(
        charmap_oldschool_black_bmp,
        charmap_oldschool_black_bmp_len,
        r);
}

void _drawText(const char* text, int x, int y, SDL_Renderer* r, SDL_Texture* font) {
    SDL_FRect src;
    SDL_FRect dest;

    src.w = BMP_CHAR_WIDTH;
    src.h = BMP_CHAR_HEIGHT;
    dest.w = FONT_WIDTH;
    dest.h = FONT_HEIGHT;
    dest.y = y;

    int offset = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        int true_index = text[i] - 32;
        
        if (text[i] == '\n') {
            offset = 0;
            dest.y += _newline(1);
            continue;
        } else if (true_index <= 0) {
            offset++;
            continue;
        }
        int x1 = true_index % BMP_CHAR_ROW_LEN;
        int y1 = true_index / BMP_CHAR_ROW_LEN;
        src.x = (x1 * BMP_CHAR_WIDTH);
        src.y = (y1 * BMP_CHAR_HEIGHT);
        dest.x = x + (FONT_WIDTH * offset);
        SDL_RenderTexture(r, font, &src, &dest);
        offset++;
    }
}
int _newline(int i) {
    return FONT_HEIGHT * i;
}



std::unique_ptr<InputManager> initializeNew(const std::string config_filepath) {
    SDL_Init(SDL_INIT_JOYSTICK);

    SDL_Renderer* r;
    SDL_Window* w;
    w = SDL_CreateWindow("BT4H Controller Setup", 640, 420, 0);
    r = SDL_CreateRenderer(w, "software");
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);


    SDL_Texture* font = _LoadFontTex(r);

    int curState = -1;
    JoystickBinding j;
    KeyboardBinding k = KEYBOARD_DEFAULT;
    int device = -2;

    std::vector<SDL_Joystick*> sticks;
    SDL_JoystickID* jIDs = SDL_GetJoysticks(nullptr);
    for(int i = 0; jIDs[i] != 0; i++) {
        sticks.push_back(SDL_OpenJoystick(jIDs[i]));
        std::cout << i << std::endl;
    }
    SDL_free(jIDs);

    while(true) {
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                goto end_initialize_new;             
            } else if (e.type == SDL_EVENT_KEY_DOWN) {
                if (curState != -1 && device == -1) {
                    k[curState] = e.key.scancode;
                    curState++;
                    SDL_FlushEvent(SDL_EVENT_KEY_DOWN);
                } else if (device == -2) {
                    device = -1;
                    curState = 0;
                }
            } else if (e.type == SDL_EVENT_JOYSTICK_BUTTON_DOWN) {
                if(curState != -1 && device != -1) {
                    j.indices[curState] = e.jbutton.button;
                    j.types[curState] = JoystickInputTypes::BUTTON;
                    curState++;
                } else if (device == -2) {
                    device = e.jbutton.which;
                    curState = 0;
                }
            } else if (e.type == SDL_EVENT_JOYSTICK_AXIS_MOTION) {
                if(abs(e.jaxis.value) < static_cast<unsigned>(SDL_JOYSTICK_AXIS_MAX * DEADZONE)) continue;
                if(curState != -1 && device != -1) {
                    j.indices[curState] = e.jaxis.axis;
                    if(e.jaxis.value >= 0) {
                        j.types[curState] = JoystickInputTypes::AXIS_POSITIVE;
                    } else {
                        j.types[curState] = JoystickInputTypes::AXIS_NEGATIVE;
                    }
                    curState++;
                    SDL_FlushEvent(SDL_EVENT_JOYSTICK_AXIS_MOTION);
                } else if (device == -2) {
                    device = e.jaxis.which;
                    curState = 0;
                }
            }
        }

        SDL_RenderClear(r);
        switch (curState) {
        case -1:
            _drawText("Welcome to the BT4H controller setup wizard.", 0, 0, r, font);
            _drawText("Please press any button on the device you wish to set up.", 
                0, _newline(1), r, font);;
            break;
        case 0:
            _drawText("Please press the A button on your device:", 10, 10, r, font);
            break;
        case 1:
            _drawText("Please press the B button on your device:", 10, 10, r, font);
            break;
        case 2:
            _drawText("Please press UP on your device:", 10, 10, r, font);
            break;
        case 3:
            _drawText("Please press DOWN on your device:", 10, 10, r, font);
            break;
        case 4:
            _drawText("Please press LEFT on your device:", 10, 10, r, font);
            break;
        case 5:
            _drawText("Please press RIGHT on your device:", 10, 10, r, font);
            break;
        case 6:
            _drawText("Please press SELECT on your device:", 10, 10, r, font);
            break;
        case 7:
            _drawText("Please press START on your device:", 10, 10, r, font);
            break;
        default:
            goto end_initialize_new;
        }
        SDL_RenderPresent(r);
        SDL_Delay(100);
    }
    end_initialize_new:

    SDL_RenderClear(r);
    _drawText(
        "Note: There are graphical issues with the BT4H\n" 
        "library on some platforms (such as KDE linux on x11).\n"
        "\n"
        "If you are seeing this message you are on such\n"
        "a platform.\n"
        "\n"
        "Your joystick seup is complete.\n"
        "Please close this app and reopen.",
        10, 10, r, font
    );
    SDL_RenderPresent(r);

    SDL_DestroyTexture(font);
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);

    SDL_GUID g;
    if(device >= 0) {
        g = SDL_GetJoystickGUIDForID(device);
    }

    for(auto stick : sticks) {
        SDL_CloseJoystick(stick);
    }
    
    if (device == -1) {  // Keyboard
        SaveLoad::SaveOrLoadKeyboardConfig(&k, true, config_filepath);
        return std::make_unique<KeyboardManager>(config_filepath);
    } else if (device != -2) {  // Just to be sure there is a device set
        SaveLoad::SaveOrLoadJoystickConfig(&j, g, true, config_filepath);
        return std::make_unique<JoystickManager>(g, config_filepath);
    }

    return nullptr;    
}

void loadAll(std::vector<std::unique_ptr<InputManager>>& vec, std::string appname) {
    vec.clear();
    vec.emplace_back(fromGUID(KEYBOARD, appname));

    SDL_JoystickID* jIDs = SDL_GetJoysticks(nullptr);
    for(int i = 0; jIDs[i] != 0; i++) {
        std::cout << "joystick " << i << std::endl;
        auto j = fromGUID(SDL_GetJoystickGUIDForID(i), appname);
        if(j != nullptr) {
            vec.emplace_back(std::move(j));
        }
    }
    SDL_free(jIDs);
}


}

}
