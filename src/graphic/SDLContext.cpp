#include "SDLContext.h"
#include "../core/Constantes.h"

#include <cassert>
#include <iostream>

using namespace std;

/**
 * @brief Initialise SDL, la fenêtre, le renderer et la musique de fond.
 */
SDLContext::SDLContext()
    : fenetre(nullptr),
      renderer(nullptr),
      musique(nullptr)
{
    assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0);

    assert((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != 0);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cerr << "Erreur Mix_OpenAudio : " << Mix_GetError() << endl;
    }
    else
    {
        musique = Mix_LoadMUS("assets/musiques/musique_mario.mp3");

        if (musique == nullptr)
        {
            cerr << "Erreur : impossible de charger la musique." << endl;
            cerr << Mix_GetError() << endl;
        }
        else
        {
            Mix_VolumeMusic(45);
            Mix_PlayMusic(musique, -1);
        }
    }

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

/**
 * @brief Libère SDL, les textures, la musique et les systèmes utilisés.
 */
SDLContext::~SDLContext()
{
    if (musique != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(musique);
        musique = nullptr;
    }

    Mix_CloseAudio();

    if (renderer != nullptr)
        SDL_DestroyRenderer(renderer);

    if (fenetre != nullptr)
        SDL_DestroyWindow(fenetre);

    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}

/**
 * @brief Retourne le renderer SDL.
 */
SDL_Renderer* SDLContext::getRenderer() const
{
    return renderer;
}

/**
 * @brief Retourne la fenêtre SDL.
 */
SDL_Window* SDLContext::getFenetre() const
{
    return fenetre;
}