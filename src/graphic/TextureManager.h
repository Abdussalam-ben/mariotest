#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "SDL.h"
#include "SDL_image.h"
#include <map>
#include <string>

/**
 * @brief Gère le chargement et la réutilisation des textures.
 */
class TextureManager {
private:
    SDL_Renderer* renderer;
    std::map<std::string, SDL_Texture*> textures;

public:
    TextureManager(SDL_Renderer* r);
    ~TextureManager();

    SDL_Texture* getTexture(const std::string& chemin);
};

#endif
