#include <sago/platform_folders.h>

#include "InputManagerFactory.hpp"
#include "image.h"


constexpr int BMP_CHAR_HEIGHT = 9;
constexpr int BMP_CHAR_WIDTH = 7;
constexpr int BMP_CHAR_ROW_LEN = 18;
constexpr int FONT_HEIGHT = 16;
constexpr int FONT_WIDTH = 12;



namespace BT4H {

namespace InputManagerFactory {

std::unique_ptr<InputManager> fromGUID(SDL_GUID guid, const std::string filepath) {
    if (std::memcmp(&KEYBOARD, &guid, sizeof(SDL_GUID)) == 0) {
        return std::make_unique<KeyboardManager>(KEYBOARD_DEFAULT);
    } else {
        return NULL;
    }
}


SDL_Texture* _SDL_TexFromMemoryConstant(unsigned char* data, int len, SDL_Renderer* r) {
    SDL_IOStream* IO = SDL_IOFromMem(data, len);

    SDL_Surface* surface = SDL_LoadBMP_IO(IO, true);
    if (!surface) {
        SDL_Log("LoadBMP_RW failed: %s", SDL_GetError());
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

    for (int i = 0; text[i] != '\0'; i++) {
        int true_index = text[i] - 32;
        if (true_index <= 32) {
            continue;
        }
        int x1 = true_index % BMP_CHAR_ROW_LEN;
        int y1 = true_index / BMP_CHAR_ROW_LEN;
        src.x = (x1 * BMP_CHAR_WIDTH);
        src.y = (y1 * BMP_CHAR_HEIGHT);
        dest.x = x + (FONT_WIDTH * i);
        SDL_RenderTexture(r, font, &src, &dest);
    }
}
int _newline(int i) {
    return FONT_HEIGHT * i;
}

std::unique_ptr<InputManager> initializeNew(const std::string config_filepath) {
    SDL_Renderer* r;
    SDL_Window* w;
    SDL_CreateWindowAndRenderer(
        "BT4H Controller Setup",
        640, 420,
        0,
        &w,
        &r);

    SDL_Texture* font = _LoadFontTex(r);

    while(true) {
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if (e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                goto end_initialize_new;             
            }
        }

        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);
        _drawText("Welcome to the BT4H controller setup wizard.", 0, 0, r, font);
        _drawText("Please press any button on the device you wish to set up.", 
            0, _newline(1), r, font);
        SDL_RenderPresent(r);

    }
    end_initialize_new:

    SDL_DestroyTexture(font);
    SDL_DestroyWindow(w);
    SDL_DestroyRenderer(r);
    return std::make_unique<KeyboardManager>(KEYBOARD_DEFAULT);
}


}

}
