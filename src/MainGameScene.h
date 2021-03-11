#ifndef MAIN_GAME_SCENE
#define MAIN_GAME_SCENE

#define PI 3.14159265

#include <SDL2/SDL.h>
// #include "globals.hpp"
#include "Input.h"
#include "Text.h"
#include "SheetMap.h"

class MainGameScene {
public:
    MainGameScene(int startingLevel = 0);
    void update(float totalTime, float dt, SheetMap &sm);
    void render(SDL_Renderer* mainRenderer, TextHandler* textHandler);
    bool isFinished();
    void resetScene();
private:
    bool finished = false;
    float maximumSpeed = 0.75f;

    void handleInput(SheetMap &sm);
    bool checkCollision(std::vector<std::vector<int>> tiles, Entity* player);
    bool checkCollision(std::vector<std::vector<int>> level, float sx, float sy, float ex, float ey);

    const int SCREEN_WIDTH = 192;
    const int SCREEN_HEIGHT = 112;

    float clamp(float n, float lower, float upper) {
        return std::max(lower, std::min(n, upper));
    }

    float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }
};

#endif // MAIN_GAME_SCENE