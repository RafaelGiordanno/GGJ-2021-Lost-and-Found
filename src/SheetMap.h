#ifndef SHEET_MAP
#define SHEET_MAP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "Input.h"
#include "Entity.h"
#include "json.hpp"

class SheetMap {
public:
    static SheetMap& getInstance();

    Input* input;
    Entity* player;
    SDL_RendererFlip playerFlip = SDL_FLIP_NONE;
    SDL_Rect playerBodyRect;
    bool playerIsFlying = false;
    bool playerCollision = false;

    Mix_Music* g_titleMusic = nullptr;

    std::vector<std::vector<int>> levelTiles;

    nlohmann::json gameMap;
    std::vector<std::vector<int>> bgTiles;
    std::vector<std::vector<int>> collidableTiles;

private:
    SheetMap();
};

#endif // SHEET_MAP