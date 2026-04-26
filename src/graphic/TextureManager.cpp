#include "TextureManager.h"
#include <cassert>
#include <iostream>

using namespace std;

TextureManager::TextureManager(SDL_Renderer* r) : renderer(r) {
    assert(renderer != nullptr);
}

TextureManager::~TextureManager() {
    for (map<string, SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it) {
        if (it->second != nullptr)
            SDL_DestroyTexture(it->second);
    }

    textures.clear();
}

SDL_Texture* TextureManager::getTexture(const string& chemin) {
    if (textures.find(chemin) != textures.end())
        return textures[chemin];

    SDL_Surface* surface = IMG_Load(chemin.c_str());

    if (surface == nullptr) {
        cerr << "Erreur chargement image : " << chemin << endl;
        cerr << IMG_GetError() << endl;
    }

    assert(surface != nullptr);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    assert(texture != nullptr);

    textures[chemin] = texture;
    return texture;
}
