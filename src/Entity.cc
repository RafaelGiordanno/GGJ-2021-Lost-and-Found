#include "Entity.h"
#include "SDL2/SDL.h"

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
    SDL_QueryTexture(commonTexture, nullptr, nullptr, &w, &h);
    printf("This is the width and height of this assigned tex: %d %d\n", w, h);
    this->totalFrames = w / this->currentFrameRect->w;
    return true;
}

bool Entity::loadTexture(SDL_Renderer *renderer, std::string path) {
    SDL_Texture* newTexture;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), 
            IMG_GetError());
        return false;
    }
    // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == nullptr) {
        printf("Unable to create texture: %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }
    this->m_texture = newTexture;
    this->totalFrames = loadedSurface->w / this->currentFrameRect->w;
    SDL_FreeSurface(loadedSurface);
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