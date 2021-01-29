#ifndef ENTITY_H
#define ENTITY_H
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Vec2f.h"

enum ItemDirection {
    RIGHT, LEFT, UP, DOWN
};

enum EntityType {
    t_PLAYER, t_HANDLE, t_BUTTON, t_DOOR, 
    t_VERTICAL_PLATFORM, t_HORIZONTAL_PLATFORM,
    t_BUTTON_PLATFORM, t_DIRECTION_ARROW
};

class Entity {
public:
    Entity(float x = 0.0f, float y = 0.0f, int w = 16.0f, int h = 16.0f, float r = 16.0f);
    bool loadTexture(SDL_Renderer*, std::string);
    bool assignTexture(SDL_Texture* commonTexture);
    void update(float);
    void setFrameRate(float);

    Vec2f* pos;
    Vec2f* startingPos;
    Vec2f* vel;
    float radius;
    double angle;
    SDL_Texture* m_texture;
    SDL_Rect* currentFrameRect;
    SDL_Rect* outputFrameRect;
    SDL_Point* center;
    EntityType type;

    int frame = 0;
    int totalFrames;
    float frameRate;
    float frameTime;
};
#endif // ENTITY_H