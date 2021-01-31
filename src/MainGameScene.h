#ifndef MAIN_GAME_SCENE
#define MAIN_GAME_SCENE

#include <SDL2/SDL.h>

class MainGameScene {
public:
    MainGameScene(int startingLevel = 0);
    void update(float dt, float totalTime);
    void render(SDL_Renderer* mainRenderer);
    bool isFinished();
private:
    bool finished = false;
};

#endif // MAIN_GAME_SCENE