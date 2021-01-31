#include "AssetManager.h"

AssetManager::AssetManager() {

}

AssetManager& AssetManager::getInstance()
{
    static AssetManager assetManager;
    return assetManager;
}

bool AssetManager::loadTexture(SDL_Renderer *renderer, std::string path) {
    if (assetMap[path]) return false;
    SDL_Texture* newTexture;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), 
            IMG_GetError());
        return false;
    }
    // SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
    // newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    assetMap[path] = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == nullptr) {
        printf("Unable to create texture: %s! SDL_Error: %s\n", path.c_str(), SDL_GetError());
        return false;
    }
    // this->m_texture = newTexture;
    // this->totalFrames = loadedSurface->w / this->currentFrameRect->w;
    printf("Tex width %d and height %d\n", loadedSurface->w, loadedSurface->h);
    SDL_FreeSurface(loadedSurface);
    // assetMap.emplace(path, newTexture);
    printf("Successfully loaded image %s!\n", path.c_str());
    return true;
}

SDL_Texture* AssetManager::getTexture(SDL_Renderer *renderer, std::string path) {
    if (!assetMap[path]) {
        printf("Texture not found, generating one!\n");
        loadTexture(renderer, path);
    }
    int w, h;
    int ret = SDL_QueryTexture(assetMap[path], nullptr, nullptr, &w, &h);
    printf("Querying the texture for validation, return: %d\n", ret);
    return assetMap[path];
}