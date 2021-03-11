#ifndef SHEET_MAP
#define SHEET_MAP

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "Vec2f.h"
#include "Input.h"
#include "Entity.h"
#include "json.hpp"

class SheetMap {
public:
    static SheetMap& getInstance();

    Input* input;
    Entity* player;
    Entity* directionArrow;
    SDL_RendererFlip playerFlip = SDL_FLIP_NONE;
    SDL_Rect playerBodyRect;
    bool playerIsFlying = false;
    bool playerCollision = false;

    Mix_Music* g_titleMusic = nullptr;

    std::vector<std::vector<int>> levelTiles;

    nlohmann::json gameMap;
    std::vector<std::vector<int>> bgTiles;
    std::vector<std::vector<int>> collidableTiles;

    std::unordered_map<std::string, Entity*> levelObjects;
    Vec2f cameraPosition;
    Vec2f directionInput;

private:
    SheetMap();
};

#endif // SHEET_MAP