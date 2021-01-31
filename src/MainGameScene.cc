#include "MainGameScene.h"

MainGameScene::MainGameScene(int startingLevel) {

}

void MainGameScene::update(float totalTime, float dt, Input* input) {

}

void MainGameScene::render(SDL_Renderer* mainRenderer, TextHandler* textHandler) {
    textHandler->render(mainRenderer, "Game started!", 58, 10);
}

bool MainGameScene::isFinished() {
    return finished;
}


void MainGameScene::handleInput(Input* input) {
    SDL_Event e;
    // event handling
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
    while (SDL_PollEvent(&e) != 0) {
        // if (e.key.keysym.sym == SDLK_z || e.key.keysym.sym == SDLK_KP_ENTER) {
        //     printf("quitting...\n");
        //     finished = true;
        // }
    }
}

void MainGameScene::resetScene() {
    finished = false;
}