#ifndef MAIN_MENU_SCENE
#define MAIN_MENU_SCENE

#include <SDL2/SDL.h>
#include "AssetManager.h"
#include "Input.h"
#include "Text.h"
#include "SheetMap.h"

class MainMenuScene {
public:
    MainMenuScene();
    void update(float dt, float totalTime);
    void render(SDL_Renderer* mainRenderer, TextHandler* textHandler);
    bool isFinished();
    void resetScene();
private:
    bool finished = false;
    SheetMap sm = SheetMap::getInstance();

    void handleInput();
};

#endif // MAIN_GAME_SCENE