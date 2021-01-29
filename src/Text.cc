#include "Text.h"

TextHandler::TextHandler(std::string fontPath) {
    initFont(fontPath.c_str(), 15);
}

void TextHandler::initFont(std::string path, int size) {
    g_font = TTF_OpenFont(path.c_str(), size);
    if (g_font == nullptr) {
        printf("Failed to load font! SDL_ttf error: %s\n", TTF_GetError());
        exit(1);
    }
}

void TextHandler::setColor(Uint8 r, Uint8 g, Uint8 b) {
    textColor = { r, g, b };
}

void TextHandler::render(SDL_Renderer* renderer, std::string txt, int x = 0, int y = 0) {
    if (txt.empty()) { return; }
    bool changed = (txt != lastText);
    if (changed) {
        lastText = txt;
        textSurface = TTF_RenderUTF8_Solid(g_font, txt.c_str(), textColor);
    }
    if (textSurface == nullptr) {
        printf("Unable to render surface! SDL_ttf error: %s\n", TTF_GetError());
    } else if (changed) {
        sampleText = SDL_CreateTextureFromSurface(renderer, textSurface);
    }
    sampleTextRect.x = x;
    sampleTextRect.y = y;
    sampleTextRect.w = textSurface->w;
    sampleTextRect.h = textSurface->h;
    if (sampleText == nullptr) {
        printf("Unable to create texture from rendered text! SDL_Error: %s\n", SDL_GetError());
    } else {
        SDL_RenderCopy(renderer, sampleText, nullptr, &sampleTextRect);
    }
}