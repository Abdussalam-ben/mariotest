#include "SDLContext.h"
#include "TextureManager.h"
#include "GameGraphicMode.h"

#include "../core/Jeu.h"
#include "../core/Joueur.h"
#include "../core/Niveau.h"
#include "../core/Vec2.h"
#include "../core/Item.h"
#include "../core/Ennemi.h"
#include "../core/PlateformeMobile.h"
#include "../core/Tuile.h"
#include "../core/Constantes.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

/**
 * @brief Structure contenant les choix du menu.
 */
struct ChoixMenu
{
    Personnage personnage;
    unsigned int niveau;
};

/**
 * @brief Convertit une position de grille en position pixel.
 */
static Vec2 posGrille(const unsigned int x, const unsigned int y)
{
    return Vec2(static_cast<float>(x) * 16.f,
                static_cast<float>(y) * 16.f);
}

/**
 * @brief Charge un niveau depuis un fichier.
 */
static Niveau chargerNiveau(const string& chemin)
{
    ifstream fichier(chemin.c_str());

    if (!fichier.is_open())
    {
        cerr << "Erreur : impossible d'ouvrir " << chemin << endl;
        exit(1);
    }

    unsigned int id;
    unsigned int largeur;
    unsigned int hauteur;

    fichier >> id >> largeur >> hauteur;

    Niveau n(id, largeur, hauteur);

    string ligne;
    getline(fichier, ligne);

    for (unsigned int y = 0; y < hauteur; y++)
    {
        getline(fichier, ligne);

        for (unsigned int x = 0; x < largeur && x < ligne.size(); x++)
        {
            char c = ligne[x];

            if (c == '#')
                n.setTuile(x, y, TypeTuile::incassable);
            else if (c == 'B')
                n.setTuile(x, y, TypeTuile::cassable);
            else if (c == 'T')
                n.setTuile(x, y, TypeTuile::tuyau);
            else if (c == '?')
                n.setTuile(x, y, TypeTuile::mystere);
            else if (c == 'S')
                n.setSpawn(posGrille(x, y));
            else if (c == 'F')
                n.setFin(posGrille(x, y));
            else if (c == 'E')
                n.ajouteEnnemi(Ennemi(posGrille(x, y), TypeEnnemi::Goomba));
            else if (c == 'K')
                n.ajouteEnnemi(Ennemi(posGrille(x, y), TypeEnnemi::Koopa));
            else if (c == 'C')
                n.ajouteItem(Item(posGrille(x, y), TypeItem::piece));
            else if (c == 'M')
                n.ajouteItem(Item(posGrille(x, y), TypeItem::champignon));
            else if (c == 'L')
                n.ajouteItem(Item(posGrille(x, y), TypeItem::fleur));
            else if (c == 'A')
                n.ajouteItem(Item(posGrille(x, y), TypeItem::etoile));
            else if (c == 'P')
                n.ajoutePlateforme(PlateformeMobile(posGrille(x, y), 48.f, AxePlateforme::horizontal));
            else
                n.setTuile(x, y, TypeTuile::vide);
        }
    }

    fichier.close();

    return n;
}

/**
 * @brief Retourne le fichier de niveau selon l'identifiant.
 */
static string cheminNiveau(const unsigned int id)
{
    if (id == 0)
        return "assets/niveaux/level0.txt";

    if (id == 1)
        return "assets/niveaux/level1.txt";

    if (id == 2)
        return "assets/niveaux/level2.txt";

    if (id == 3)
        return "assets/niveaux/level3.txt";

    if (id == 4)
        return "assets/niveaux/level4_difficile.txt";

    if (id == 5)
        return "assets/niveaux/level5_long.txt";

    return "assets/niveaux/level0.txt";
}

/**
 * @brief Charge une police simple.
 */
static TTF_Font* chargerPolice()
{
    const char* chemins[] = {
        "/usr/share/fonts/dejavu-sans-fonts/DejaVuSerif.ttf",
        "/usr/share/fonts/dejavu/DejaVuSerif.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf",
        "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans.ttf",
        "/usr/share/fonts/dejavu/DejaVuSans.ttf"
    };

    TTF_Font* police = nullptr;

    for (unsigned int i = 0; i < 5 && police == nullptr; i++)
    {
        police = TTF_OpenFont(chemins[i], 10);
    }

    return police;
}

/**
 * @brief Affiche un texte.
 */
static void afficherTexte(SDL_Renderer* renderer,
                          TTF_Font* police,
                          const string& texte,
                          int x,
                          int y,
                          SDL_Color couleur)
{
    if (police == nullptr)
        return;

    SDL_Surface* surface = TTF_RenderText_Blended(police, texte.c_str(), couleur);

    if (surface == nullptr)
        return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == nullptr)
    {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = surface->w;
    dst.h = surface->h;

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

/**
 * @brief Retourne le nom affiché du niveau.
 */
static string nomNiveauMenu(unsigned int id)
{
    if (id == 4)
        return "Niveau 4 difficile";

    if (id == 5)
        return "Niveau 5 long";

    return "Niveau " + to_string(id);
}

/**
 * @brief Affiche le menu et retourne le choix du joueur.
 */
static ChoixMenu afficherMenu(SDLContext& contexte)
{
    SDL_Renderer* renderer = contexte.getRenderer();

    if (TTF_WasInit() == 0)
    {
        if (TTF_Init() == -1)
        {
            cerr << "Erreur TTF_Init : " << TTF_GetError() << endl;

            ChoixMenu choixDefaut;
            choixDefaut.personnage = Personnage::mario;
            choixDefaut.niveau = 0;
            return choixDefaut;
        }
    }

    TTF_Font* police = chargerPolice();

    bool actif = true;
    unsigned int choixLigne = 0;
    unsigned int choixPersonnage = 0;
    unsigned int choixNiveau = 0;

    while (actif)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                if (police != nullptr)
                    TTF_CloseFont(police);

                exit(0);
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (police != nullptr)
                        TTF_CloseFont(police);

                    exit(0);
                }

                if (event.key.keysym.sym == SDLK_UP)
                {
                    if (choixLigne > 0)
                        choixLigne--;
                }

                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    if (choixLigne < 2)
                        choixLigne++;
                }

                if (event.key.keysym.sym == SDLK_LEFT)
                {
                    if (choixLigne == 0)
                    {
                        if (choixPersonnage == 0)
                            choixPersonnage = 1;
                        else
                            choixPersonnage = 0;
                    }
                    else if (choixLigne == 1)
                    {
                        if (choixNiveau == 0)
                            choixNiveau = 5;
                        else
                            choixNiveau--;
                    }
                }

                if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    if (choixLigne == 0)
                    {
                        if (choixPersonnage == 0)
                            choixPersonnage = 1;
                        else
                            choixPersonnage = 0;
                    }
                    else if (choixLigne == 1)
                    {
                        if (choixNiveau == 5)
                            choixNiveau = 0;
                        else
                            choixNiveau++;
                    }
                }

                if (event.key.keysym.sym == SDLK_RETURN ||
                    event.key.keysym.sym == SDLK_SPACE)
                {
                    if (choixLigne == 2)
                        actif = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
        SDL_RenderClear(renderer);

        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Color jaune = {255, 230, 80, 255};

        SDL_Color couleurPerso = choixLigne == 0 ? jaune : blanc;
        SDL_Color couleurNiveau = choixLigne == 1 ? jaune : blanc;
        SDL_Color couleurStart = choixLigne == 2 ? jaune : blanc;

        string nomPerso = choixPersonnage == 0 ? "Mario" : "Luigi";

        afficherTexte(renderer, police, "SUPER MARIO++", 70, 35, blanc);

        afficherTexte(renderer,
                      police,
                      "Personnage: < " + nomPerso + " >",
                      42,
                      85,
                      couleurPerso);

        afficherTexte(renderer,
                      police,
                      "Niveau: < " + nomNiveauMenu(choixNiveau) + " >",
                      42,
                      110,
                      couleurNiveau);

        afficherTexte(renderer,
                      police,
                      "Commencer",
                      92,
                      145,
                      couleurStart);

        afficherTexte(renderer,
                      police,
                      "Fleches: choisir    Entree/Espace: valider",
                      12,
                      205,
                      blanc);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    ChoixMenu choix;

    if (choixPersonnage == 0)
        choix.personnage = Personnage::mario;
    else
        choix.personnage = Personnage::luigi;

    choix.niveau = choixNiveau;

    if (police != nullptr)
        TTF_CloseFont(police);

    return choix;
}

/**
 * @brief Affiche l'écran de victoire ou de défaite.
 */
static void afficherEcranFin(SDLContext& contexte,
                             EtatPartie etat,
                             unsigned int score)
{
    SDL_Renderer* renderer = contexte.getRenderer();

    if (TTF_WasInit() == 0)
    {
        if (TTF_Init() == -1)
        {
            cerr << "Erreur TTF_Init : " << TTF_GetError() << endl;
            return;
        }
    }

    TTF_Font* police = chargerPolice();

    bool actif = true;

    while (actif)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                if (police != nullptr)
                    TTF_CloseFont(police);

                exit(0);
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (police != nullptr)
                        TTF_CloseFont(police);

                    exit(0);
                }

                if (event.key.keysym.sym == SDLK_RETURN ||
                    event.key.keysym.sym == SDLK_SPACE)
                {
                    actif = false;
                }
            }
        }

        if (etat == EtatPartie::gagnee)
            SDL_SetRenderDrawColor(renderer, 70, 170, 90, 255);
        else
            SDL_SetRenderDrawColor(renderer, 130, 40, 40, 255);

        SDL_RenderClear(renderer);

        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Color jaune = {255, 230, 80, 255};

        if (etat == EtatPartie::gagnee)
        {
            afficherTexte(renderer, police, "VICTOIRE !", 83, 70, jaune);
            afficherTexte(renderer, police, "Tu as atteint le drapeau.", 45, 105, blanc);
        }
        else
        {
            afficherTexte(renderer, police, "GAME OVER", 78, 70, jaune);
            afficherTexte(renderer, police, "Tu as perdu tes vies.", 55, 105, blanc);
        }

        afficherTexte(renderer,
                      police,
                      "Score: " + to_string(score),
                      88,
                      135,
                      blanc);

        afficherTexte(renderer,
                      police,
                      "Entree / Espace: continuer",
                      45,
                      180,
                      blanc);

        afficherTexte(renderer,
                      police,
                      "Echap: quitter",
                      82,
                      205,
                      blanc);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    if (police != nullptr)
        TTF_CloseFont(police);
}

/**
 * @brief Point d'entrée du mode graphique.
 */
int main()
{
    SDLContext contexte;
    TextureManager textures(contexte.getRenderer());

    bool applicationActive = true;

    while (applicationActive)
    {
        ChoixMenu choix = afficherMenu(contexte);

        Niveau niveau = chargerNiveau(cheminNiveau(choix.niveau));

        Joueur joueur(niveau.getSpawn());
        joueur.setPersonnage(choix.personnage);

        Jeu jeu(niveau, joueur);

        GameGraphicMode mode(contexte, textures);
        mode.setJeu(jeu, niveau, joueur);
        mode.boucle();

        afficherEcranFin(contexte, jeu.getEtat(), jeu.getScore());
    }

    return 0;
}