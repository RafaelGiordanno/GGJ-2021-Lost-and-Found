#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include "Entity.h"
#include "Text.h"
#include <ctime>
#include <cmath>
#include <fstream>

#define ASSETS_FOLDER "assets/"
#define PI 3.14159265
#ifndef DEBUG
#define DEBUG false
#endif

int gameScale = 4;
const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 144;

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
SDL_Texture* g_levelTexture;
Vec2f* g_levelSize;
TextHandler* textHandler;

Entity* player;

Mix_Music* g_titleMusic = nullptr;

unsigned int lastTime = 0;
unsigned int currentTime = 0;
SDL_Rect g_fillRect = { 80, 90, 10, 10 };

SDL_RendererFlip playerFlip = SDL_FLIP_NONE;
SDL_Rect playerBodyRect;
bool playerIsFlying = false;
bool playerCollision = false;

bool quit = false;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL failed to initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    g_window = SDL_CreateWindow("GGJ2021",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH * gameScale, SCREEN_HEIGHT * gameScale,
        SDL_WINDOW_SHOWN);
    if (g_window == nullptr) {
        printf("Failed to initialize window! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == nullptr) {
        printf("Failed to initialize renderer! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_RenderSetScale(g_renderer, gameScale, gameScale);
    int imgFlags =  IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("PNG loader failed! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

void close() {
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    Mix_FreeMusic(g_titleMusic);
    g_titleMusic = nullptr;
    g_renderer = nullptr;
    g_window = nullptr;
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

//===== MATH BEGINS
float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

// MATH ENDS =======

void handleInput() {
    SDL_Event e;
    // event handling
    const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
            printf("quitting...\n");
            quit = true;
        }
    }
}

void renderEntity(Entity* ent, SDL_RendererFlip flip) {
    g_fillRect.x = ent->outputFrameRect->x;
    g_fillRect.y = ent->outputFrameRect->y;
    g_fillRect.w = ent->outputFrameRect->w;
    g_fillRect.h = ent->outputFrameRect->h;
    SDL_RenderCopyEx(g_renderer, ent->m_texture, ent->currentFrameRect, &g_fillRect,
        ent->angle, ent->center, flip);
}

void update(float dt) {
    handleInput();
}

void render() {
    SDL_SetRenderDrawColor(g_renderer, 0x33, 0x11, 0x46, 0xFF);
    SDL_RenderClear(g_renderer);
    // renderEntity(player, playerFlip);

    // text rendering
    textHandler->render(g_renderer, "Welcome to our game!", 38, 10);
    textHandler->render(g_renderer, "This is another line!", 42, 20);
    textHandler->render(g_renderer, "This is another line!", 42, 30);

    SDL_RenderPresent(g_renderer);
}

const float physicsDeltaTime = 1/60.f;
float timeAccumulator = 0.0f;
double totalGameTime = 0.0f;

void mainLoop() {
    //===== TIME BEGINS
    currentTime = SDL_GetTicks();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    timeAccumulator += dt;
    //===== TIME ENDS

    while (timeAccumulator >= physicsDeltaTime) {
        update(physicsDeltaTime);
        printf("%lf\n", timeAccumulator);
        timeAccumulator -= physicsDeltaTime;
        totalGameTime += physicsDeltaTime;
    }
    render();
}

SDL_Texture* loadTexture(SDL_Renderer *renderer, std::string path) {
    SDL_Texture* newTexture;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        exit(1);
    }
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == nullptr) {
        printf("Unable to create texture: %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
        exit(1);
    }
    return newTexture;
}

void initEntities() {
    // INITIALIZING THE PLAYER
    player = new Entity(72, 12, 8, 8);
    player->type = t_PLAYER;
    player->setFrameRate(1 / 12.f);
    if (!player->loadTexture(g_renderer, "assets/astronaut.png")) {
        printf("Error loading player texture! SDL_Error: %s\n", SDL_GetError());
    }
}

void initMusic() {
    g_titleMusic = Mix_LoadMUS("assets/m_title.ogg");
    if (g_titleMusic == nullptr) {
        printf("Error loading music! Mix_Error: %s\n", Mix_GetError());
        exit(1);
    }
}

void initFont() {
    textHandler = new TextHandler("assets/m3x6.ttf");
    textHandler->setColor(0xdc, 0xfe, 0xcf);
}

int main(int argc, char** argv) {
    srand((unsigned) time(nullptr));
    if (!initSDL()) { return -1; }
    lastTime = currentTime = SDL_GetTicks();
    initEntities();
    initFont();
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, -1, 1);
#else
    while (!quit) {
        mainLoop();
    }
#endif
    close();
    return 0;
}