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

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
    return Vec2(static_cast<float>(x) * 16.f, static_cast<float>(y) * 16.f);
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
    if (id == 0) return "assets/niveaux/level0.txt";
    if (id == 1) return "assets/niveaux/level1.txt";
    if (id == 2) return "assets/niveaux/level2.txt";
    if (id == 3) return "assets/niveaux/level3.txt";
    if (id == 4) return "assets/niveaux/level4_difficile.txt";
    if (id == 5) return "assets/niveaux/level5_long.txt";

    return "assets/niveaux/level0.txt";
}

/**
 * @brief Charge la police Mario utilisée dans le menu et les écrans de fin.
 */
static TTF_Font* chargerPolice()
{
    const char* cheminPolice = "assets/polices/super_mario_bros_nes.ttf";

    TTF_Font* police = TTF_OpenFont(cheminPolice, 8);

    if (police == nullptr)
    {
        cerr << "Erreur : impossible de charger la police " << cheminPolice << endl;
        cerr << TTF_GetError() << endl;
    }

    return police;
}

/**
 * @brief Affiche un texte à une position donnée.
 */
static void afficherTexte(SDL_Renderer* renderer, TTF_Font* police,
                          const string& texte, int x, int y, SDL_Color couleur)
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
 * @brief Affiche un texte centré horizontalement.
 */
static void afficherTexteCentre(SDL_Renderer* renderer, TTF_Font* police,
                                const string& texte, int y, SDL_Color couleur)
{
    if (police == nullptr)
        return;

    int largeurTexte = 0;
    int hauteurTexte = 0;

    if (TTF_SizeText(police, texte.c_str(), &largeurTexte, &hauteurTexte) != 0)
        return;

    int x = static_cast<int>((WINDOW_NES_WIDTH - largeurTexte) / 2);

    afficherTexte(renderer, police, texte, x, y, couleur);
}

/**
 * @brief Retourne le nom affiché du niveau.
 */
static string nomNiveauMenu(unsigned int id)
{
    if (id == 4)
        return "NIVEAU 4 DIFFICILE";

    if (id == 5)
        return "NIVEAU 5 LONG";

    return "NIVEAU " + to_string(id);
}

/**
 * @brief Affiche l'écran des commandes.
 */
static void afficherCommandes(SDLContext& contexte, TTF_Font* police)
{
    SDL_Renderer* renderer = contexte.getRenderer();

    bool actif = true;

    while (actif)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                exit(0);
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE ||
                    event.key.keysym.sym == SDLK_RETURN ||
                    event.key.keysym.sym == SDLK_SPACE)
                {
                    actif = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
        SDL_RenderClear(renderer);

        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Color jaune = {255, 230, 80, 255};

        afficherTexteCentre(renderer, police, "COMMANDES", 24, jaune);

        afficherTexteCentre(renderer, police, "Q / GAUCHE", 58, blanc);
        afficherTexteCentre(renderer, police, "ALLER A GAUCHE", 72, blanc);

        afficherTexteCentre(renderer, police, "D / DROITE", 96, blanc);
        afficherTexteCentre(renderer, police, "ALLER A DROITE", 110, blanc);

        afficherTexteCentre(renderer, police, "ESPACE / Z / HAUT", 134, blanc);
        afficherTexteCentre(renderer, police, "SAUTER", 148, blanc);

        afficherTexteCentre(renderer, police, "F: TIRER", 172, blanc);
        afficherTexteCentre(renderer, police, "P: PAUSE", 186, blanc);

        afficherTexteCentre(renderer, police, "ENTREE: RETOUR", 216, jaune);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
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
                    if (choixLigne < 3)
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
                    {
                        afficherCommandes(contexte, police);
                    }

                    if (choixLigne == 3)
                    {
                        actif = false;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255);
        SDL_RenderClear(renderer);

        SDL_Color blanc = {255, 255, 255, 255};
        SDL_Color jaune = {255, 230, 80, 255};

        SDL_Color couleurPerso = choixLigne == 0 ? jaune : blanc;
        SDL_Color couleurNiveau = choixLigne == 1 ? jaune : blanc;
        SDL_Color couleurCommandes = choixLigne == 2 ? jaune : blanc;
        SDL_Color couleurStart = choixLigne == 3 ? jaune : blanc;

        string nomPerso = choixPersonnage == 0 ? "MARIO" : "LUIGI";

        afficherTexteCentre(renderer, police, "SUPER MARIO", 20, blanc);
        afficherTexteCentre(renderer, police, "FAIT PAR P2207174", 36, blanc);

        afficherTexteCentre(renderer, police, "PERSONNAGE", 64, couleurPerso);
        afficherTexteCentre(renderer, police, "< " + nomPerso + " >", 80, couleurPerso);

        afficherTexteCentre(renderer, police, "NIVEAU", 106, couleurNiveau);
        afficherTexteCentre(renderer, police, "< " + nomNiveauMenu(choixNiveau) + " >", 122, couleurNiveau);

        afficherTexteCentre(renderer, police, "COMMANDES", 154, couleurCommandes);

        afficherTexteCentre(renderer, police, "COMMENCER", 184, couleurStart);

        afficherTexteCentre(renderer, police, "FLECHES: CHOISIR", 212, blanc);
        afficherTexteCentre(renderer, police, "ENTREE/ESPACE: OK", 228, blanc);

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
static void afficherEcranFin(SDLContext& contexte, EtatPartie etat, unsigned int score)
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
            afficherTexteCentre(renderer, police, "VICTOIRE !", 68, jaune);
            afficherTexteCentre(renderer, police, "TU AS ATTEINT", 104, blanc);
            afficherTexteCentre(renderer, police, "LE DRAPEAU.", 122, blanc);
        }
        else
        {
            afficherTexteCentre(renderer, police, "GAME OVER", 68, jaune);
            afficherTexteCentre(renderer, police, "TU AS PERDU", 104, blanc);
            afficherTexteCentre(renderer, police, "TES VIES.", 122, blanc);
        }

        afficherTexteCentre(renderer, police, "SCORE: " + to_string(score), 152, blanc);

        afficherTexteCentre(renderer, police, "ENTREE / ESPACE:", 190, blanc);
        afficherTexteCentre(renderer, police, "CONTINUER", 206, blanc);
        afficherTexteCentre(renderer, police, "ECHAP: QUITTER", 224, blanc);

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