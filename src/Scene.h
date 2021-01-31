#ifndef SCENE
#define SCENE

#include <SDL2/SDL.h>

class Scene {
public:
    Scene();
    void update(float dt, float totalTime);
    void render(SDL_Renderer* mainRenderer);
    bool isFinished();
private:
    bool finished = false;
};

#endif // SCENE