#ifndef SDLCONTEXT_H
#define SDLCONTEXT_H

#include "SDL.h"
#include "SDL_image.h"

/**
 * @brief Gère l'initialisation de SDL, la fenêtre et le renderer.
 */
class SDLContext {
private:
    SDL_Window* fenetre;
    SDL_Renderer* renderer;

public:
    SDLContext();
    ~SDLContext();

    SDL_Renderer* getRenderer() const;
    SDL_Window* getFenetre() const;
};

#endif
