#ifndef ASSET_MANAGER
#define ASSET_MANAGER

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>

class AssetManager {
public:
    static AssetManager& getInstance();
    bool loadTexture(SDL_Renderer* renderer, std::string path);
    SDL_Texture* getTexture(SDL_Renderer* renderer, std::string path);
private:
    AssetManager();
    std::unordered_map<std::string, SDL_Texture*> assetMap;
};

#endif // ASSET_MANAGER