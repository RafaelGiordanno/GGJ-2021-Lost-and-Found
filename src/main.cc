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
#include <ctime>
#include <cmath>
#include <fstream>
#include "json.hpp"

#include "AssetManager.h"
#include "Input.h"
#include "Entity.h"
#include "Text.h"
#include "MainGameScene.h"
#include "MainMenuScene.h"

#define ASSETS_FOLDER "assets/"
#define PI 3.14159265
#ifndef DEBUG
#define DEBUG false
#endif

#define INDEX_MAIN_MENU 0
#define INDEX_INTRO_CUTSCENE 1
#define INDEX_MAIN_GAME 2
#define INDEX_ENDING 3

int gameScale = 4;
const int SCREEN_WIDTH = 192;
const int SCREEN_HEIGHT = 112;

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

const float physicsDeltaTime = 1/60.f;
float timeAccumulator = 0.0f;
double totalGameTime = 0.0f;

nlohmann::json gameMap;
std::vector<std::vector<int>> bgTiles;
std::vector<std::vector<int>> collidableTiles;

Input *input;
std::vector<std::vector<int>> levelTiles;
int currentSceneIndex = INDEX_MAIN_MENU;
MainGameScene* mainGameScene;
MainMenuScene* mainMenuScene;

bool pauseGame = false;

// AssetManager assetManager;
std::unordered_map<std::string, Entity*> levelObjects;

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

void loadLevel(SDL_Renderer *renderer, std::string path) {
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
    g_levelTexture = newTexture;
    g_levelSize = new Vec2f((float) loadedSurface->w, (float) loadedSurface->h);
    printf("g_levelSize: x %d y %d\n", (int) g_levelSize->x, (int) g_levelSize->y);
    SDL_FreeSurface(loadedSurface);
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
        } else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_p) {
            pauseGame = !pauseGame;
        } else if (e.type == SDL_KEYUP) {
            input->setKeyDown(e.key.keysym.sym, false);
        } else if (e.type == SDL_KEYDOWN) {
            input->setKeyDown(e.key.keysym.sym, true);
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
    if (pauseGame) return;

    playerBodyRect.x = player->pos->x;
    playerBodyRect.y = player->pos->y;
    playerBodyRect.w = player->currentFrameRect->w;
    playerBodyRect.h = player->currentFrameRect->h;

    player->pos->x += player->vel->x;
    player->pos->y += player->vel->y;
    player->pos->x = clamp(player->pos->x, 0, SCREEN_WIDTH);
    player->pos->y = clamp(player->pos->y, 0, SCREEN_HEIGHT);
    player->frame = 0;
    player->update(dt);

    for (auto obj : levelObjects) {
    //   cout << x.first << " " << x.second << endl; 
        obj.second->pos->x += obj.second->vel->x;
        obj.second->pos->y += obj.second->vel->y;
        obj.second->pos->x = clamp(obj.second->pos->x, 0, SCREEN_WIDTH);
        obj.second->pos->y = clamp(obj.second->pos->y, 0, SCREEN_HEIGHT);
        obj.second->frame = 0;
        obj.second->update(dt);
     }

    // GAME STRUCTURE
    // - MENU (global, ruled by the main file)
    if (currentSceneIndex == INDEX_MAIN_MENU) {
        totalGameTime = 0.0f;
        mainMenuScene->update(totalGameTime, dt, input);
        if (mainMenuScene->isFinished()) {
            printf("Main Menu Scene finished! Going to the next scene!\n");
            currentSceneIndex++;
            mainMenuScene->resetScene();
        }
        return;
    }
    // - INTRO CUTSCENE (go to this scene)
    if (currentSceneIndex == INDEX_INTRO_CUTSCENE) {
        totalGameTime = 0.0f;
        return;
    }
    // - LEVELS (all levels are in another scene)
    if (currentSceneIndex == INDEX_MAIN_GAME) {
        mainGameScene->update(totalGameTime, dt, input);
        if (mainGameScene->isFinished()) {
            printf("Main Menu Scene finished! Going to the next scene!\n");
            currentSceneIndex++;
            mainGameScene->resetScene();
        }
        return;
    }
    // - ENDING (this is another scene as well) 
    if (currentSceneIndex == INDEX_ENDING) {
        return;
    }
    // - BACK TO MAIN MENU
    currentSceneIndex = INDEX_MAIN_MENU;
}

void renderLevelTiles() {
    SDL_Rect* tilePosition = new SDL_Rect();
    int tileSize = 8;
    tilePosition->w = tileSize;
    tilePosition->h = tileSize;
    int tw = (int) (g_levelSize->x) / tileSize;
    int th = (int) (g_levelSize->y) / tileSize;

    g_fillRect.w = tileSize;
    g_fillRect.h = tileSize;
    // TODO limit tile rendering to visible tiles only
    for (int i = 0; i < collidableTiles.size(); i++) {
        g_fillRect.y = i * tileSize; // - cameraPosition.x;
        for (int k = 0; k < collidableTiles[i].size(); k++) {
            if (!collidableTiles[i][k]) continue;
            // printf("values: i: %d k: %d\n", i, k);
            g_fillRect.x = k * tileSize; // - cameraPosition.y;
            tilePosition->x = ((collidableTiles[i][k] - 1) % tw) * tileSize;
            tilePosition->y = ((collidableTiles[i][k] - 1) / tw) * tileSize;
            // printf("collidableTiles[%d][%d] %d \ntilePosition x %d y %d\n\n", i, k, collidableTiles[i][k], tilePosition->x, tilePosition->y);
            SDL_RenderCopyEx(g_renderer, g_levelTexture, tilePosition, &g_fillRect,
                0.0, nullptr, SDL_FLIP_NONE);
        }
    }
}

void render() {
    SDL_SetRenderDrawColor(g_renderer, 0x33, 0x11, 0x46, 0xFF);
    SDL_RenderClear(g_renderer);

    renderLevelTiles();
    renderEntity(player, playerFlip);
    renderEntity(levelObjects["door_01"], playerFlip);

    // - MENU (global, ruled by the main file)
    if (currentSceneIndex == INDEX_MAIN_MENU) {
        mainMenuScene->render(g_renderer, textHandler);
    } else if (currentSceneIndex == INDEX_INTRO_CUTSCENE) {
    } else if (currentSceneIndex == INDEX_MAIN_GAME) {
        mainGameScene->render(g_renderer, textHandler);
    }

    if (pauseGame) {
        SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 - 12, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4 };
        SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(g_renderer, &fillRect);
        textHandler->render(g_renderer, "Game Paused", fillRect.x + 26, fillRect.y + 2);
        textHandler->render(g_renderer, "Press P to Unpause", fillRect.x + 12, fillRect.y + 12);
    }
    
    SDL_RenderPresent(g_renderer);
}

void mainLoop() {
    //===== TIME BEGINS
    currentTime = SDL_GetTicks();
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    timeAccumulator += dt;
    //===== TIME ENDS

    while (timeAccumulator >= physicsDeltaTime) {
        update(physicsDeltaTime);
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
    player = new Entity(72, 94, 8, 8);
    player->type = t_PLAYER;
    player->setFrameRate(1 / 12.f);
    player->assignTexture(AssetManager::getInstance().getTexture(g_renderer, "assets/astronaut.png"));
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

std::vector<std::vector<int>> convertMap(std::vector<int> originalMap, int w, int h) {
    std::vector<std::vector<int>> convertedMap;

    int index = 0;
    for (int i = 0; i < h; i++) {
        convertedMap.push_back(std::vector<int>());
        for (int k = 0; k < w; k++) {
            if (originalMap[index]) { printf("map index: %d i: %d k: %d\n", index, i, k);}
            convertedMap[i].push_back(originalMap[index++]);
        }
    }

    return convertedMap;
}

void initMap() {
    printf("Loading map: mainMap.json\n");
    std::ifstream savedMap("assets/main_map.json");
    if (savedMap.good()) {
        printf("Map loaded successfully!\n");
        savedMap >> gameMap;
    } else {
        printf("Map not found!\n");
    }
    savedMap.close();
    printf("First map!");
    bgTiles = convertMap(gameMap["layers"][0]["data"].get<std::vector<int>>(), 240, 140);
    printf("Second map!");
    collidableTiles = convertMap(gameMap["layers"][1]["data"].get<std::vector<int>>(), 240, 140);
    printf("Initializing level objects\n");
    nlohmann::json objectsArray = gameMap["layers"][2]["objects"];
    for (int k = 0; k < objectsArray.size(); k++) {
        std::string name = objectsArray[k]["name"].get<std::string>();
        std::string type = objectsArray[k]["type"].get<std::string>();
        int x = objectsArray[k]["x"].get<int>();
        int y = objectsArray[k]["y"].get<int>();
        int width = objectsArray[k]["width"].get<int>();
        int height = objectsArray[k]["height"].get<int>();
        if (name.find("door") != std::string::npos) {
            // create new entity based on the name "door_" + type
            levelObjects[name] = new Entity(x, y - 8, width, height);
            levelObjects[name]->assignTexture(AssetManager::getInstance().getTexture(g_renderer, std::string("assets/door_").append(type).append(".png")));
        }
    }
}

void initScenes() {
    input = new Input();
    mainMenuScene = new MainMenuScene();
    mainGameScene = new MainGameScene(0);
}

int main(int argc, char** argv) {
    srand((unsigned) time(nullptr));
    if (!initSDL()) { return -1; }
    lastTime = currentTime = SDL_GetTicks();
    loadLevel(g_renderer, "assets/tiles_blue.png");
    initEntities();
    initFont();
    initMap();
    initScenes();
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