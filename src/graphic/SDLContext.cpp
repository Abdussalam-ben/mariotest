#include "SDLContext.h"
#include "../core/Constantes.h"
#include <cassert>

SDLContext::SDLContext() : fenetre(nullptr), renderer(nullptr) {
    assert(SDL_Init(SDL_INIT_VIDEO) == 0);
    assert((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != 0);

    fenetre = SDL_CreateWindow("Super Mario",
                               SDL_WINDOWPOS_CENTERED,
                               SDL_WINDOWPOS_CENTERED,
                               WINDOW_WIDTH,
                               WINDOW_HEIGHT,
                               SDL_WINDOW_SHOWN);
    assert(fenetre != nullptr);

    renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    assert(renderer != nullptr);

    SDL_RenderSetLogicalSize(renderer, WINDOW_NES_WIDTH, WINDOW_NES_HEIGHT);
}

SDLContext::~SDLContext() {
    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if (fenetre != nullptr)
        SDL_DestroyWindow(fenetre);

    IMG_Quit();
    SDL_Quit();
}

SDL_Renderer* SDLContext::getRenderer() const {
    return renderer;
}

SDL_Window* SDLContext::getFenetre() const {
    return fenetre;
}
