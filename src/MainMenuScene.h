#ifndef MAIN_MENU_SCENE
#define MAIN_MENU_SCENE

#include <SDL2/SDL.h>
#include "Input.h"
#include "Text.h";

class MainMenuScene {
public:
    MainMenuScene();
    void update(float dt, float totalTime, Input* input);
    void render(SDL_Renderer* mainRenderer, TextHandler* textHandler);
    bool isFinished();
    void resetScene();
private:
    bool finished = false;

    void handleInput(Input* input);
};

#endif // MAIN_GAME_SCENE