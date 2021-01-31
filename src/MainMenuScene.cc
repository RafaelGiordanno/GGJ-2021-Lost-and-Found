#include "MainMenuScene.h"

MainMenuScene::MainMenuScene() {
    
}

void MainMenuScene::update(float totalTime, float dt, Input* input) {
    handleInput(input);
}

void MainMenuScene::render(SDL_Renderer* mainRenderer, TextHandler* textHandler) {
    textHandler->render(mainRenderer, "SR-7", 78, 10);
    textHandler->render(mainRenderer, "Start the game already!", 38, 90);
}

bool MainMenuScene::isFinished() {
    return finished;
}

void MainMenuScene::handleInput(Input* input) {
    SDL_Event e;
    // event handling
    if (input->getKeyDown(Input::ACCEPT)) {
        printf("Start thsi game lareayde??\n");
        finished = true;
    }
}

void MainMenuScene::resetScene() {
    finished = false;
}