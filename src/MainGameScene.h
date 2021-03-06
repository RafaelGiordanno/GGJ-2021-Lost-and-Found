#ifndef MAIN_GAME_SCENE
#define MAIN_GAME_SCENE

#include <SDL2/SDL.h>
#include "Input.h"
#include "Text.h";

class MainGameScene {
public:
    MainGameScene(int startingLevel = 0);
    void update(float dt, float totalTime, Input* input);
    void render(SDL_Renderer* mainRenderer, TextHandler* textHandler);
    bool isFinished();
    void resetScene();
private:
    bool finished = false;

    void handleInput(Input* input);
};

#endif // MAIN_GAME_SCENE