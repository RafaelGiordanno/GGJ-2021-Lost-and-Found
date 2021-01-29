#ifndef TEXT_HPP
#define TEXT_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Vec2f.h"

class TextHandler {
public:
    TextHandler(std::string fontPath);
    void initFont(std::string, int size);
    void setColor(Uint8 r, Uint8 g, Uint8 b);
    void render(SDL_Renderer* renderer, std::string txt, int x, int y);
private:
    TTF_Font* g_font = nullptr;
    SDL_Texture* sampleText;
    SDL_Rect sampleTextRect;
    SDL_Surface* textSurface;
    SDL_Color textColor;

    std::string lastText = "";
};


#endif // TEXT_HPP