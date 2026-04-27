#ifndef SDLCONTEXT_H
#define SDLCONTEXT_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL2/SDL_mixer.h"

/**
 * @brief Gère l'initialisation de SDL, la fenêtre, le renderer et la musique.
 */
class SDLContext
{
private:
    SDL_Window* fenetre;
    SDL_Renderer* renderer;
    Mix_Music* musique;

public:
    SDLContext();
    ~SDLContext();

    SDL_Renderer* getRenderer() const;
    SDL_Window* getFenetre() const;
};

#endif