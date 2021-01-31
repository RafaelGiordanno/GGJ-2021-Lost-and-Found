#include "Entity.h"
#include "SDL2/SDL.h"
#include <iostream>

Entity::Entity(float x, float y, int w, int h, float r) {
    this->pos = new Vec2f(x, y);
    this->startingPos = new Vec2f(x, y);
    this->vel = new Vec2f();
    this->radius = r;
    this->angle = 0.0;
    this->currentFrameRect = new SDL_Rect();
    this->outputFrameRect = new SDL_Rect();
    this->currentFrameRect->w = w > 0 ? w : this->currentFrameRect->w;
    this->currentFrameRect->h = h > 0 ? h : this->currentFrameRect->h;
    this->center = nullptr;
    this->totalFrames = 1;  
}

bool Entity::assignTexture(SDL_Texture* commonTexture) {
    int w, h;
    this->m_texture = commonTexture;
    int ret = SDL_QueryTexture(commonTexture, nullptr, nullptr, &w, &h);
    // printf("This is the width and height of this assigned tex: %d %d\n", w, h);
    std::cout << "return = " << ret << " / This is the width and height of this assigned tex: " << w << " " << h << std::endl; 
    this->totalFrames = w / this->currentFrameRect->w;
    return true;
}

void Entity::update(float dt) {
    this->currentFrameRect->x = frame * this->currentFrameRect->w;

    this->outputFrameRect->x = (int) this->pos->x;
    this->outputFrameRect->y = (int) this->pos->y;
    this->outputFrameRect->w = this->currentFrameRect->w;
    this->outputFrameRect->h = this->currentFrameRect->h;
}

void Entity::setFrameRate(float frameRate) {
    this->frameRate = frameRate;
    this->frame = 0;
}