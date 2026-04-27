#ifndef GAMEGRAPHICMODE_H
#define GAMEGRAPHICMODE_H

#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SDLContext.h"
#include "TextureManager.h"

#include "../core/Jeu.h"
#include "../core/Joueur.h"
#include "../core/Niveau.h"
#include "../core/Ennemi.h"
#include "../core/Item.h"
#include "../core/Feu.h"
#include "../core/EntreeJoueur.h"
#include "../core/PlateformeMobile.h"
#include "../core/Tuile.h"
#include "../core/Constantes.h"

/**
 * @brief Gère l'affichage graphique du jeu.
 */
class GameGraphicMode
{
private:
    SDLContext* contexte;
    TextureManager* textures;

    Jeu* jeu;
    Niveau* niveau;
    Joueur* joueur;

    bool actif;
    bool enPause;

    unsigned int frame;

    float cameraX;
    float cameraY;

    TTF_Font* police;

    unsigned int scoreMax;

public:
    GameGraphicMode(SDLContext& c, TextureManager& t);

    void setJeu(Jeu& j, Niveau& n, Joueur& p);

    EntreeJoueur lireEntree() const;

    void majCamera();

    void initialiserPolice();
    void fermerPolice();

    void afficherSprite(const char* nom, int x, int y, int l, int h, bool flip = false);

    void afficherTexte(const std::string& texte, int x, int y);
    void afficherTexteCentre(const std::string& texte, int y);

    void afficherHUD();
    void afficherPause();

    std::string nomJoueur() const;
    std::string nomNiveau() const;
    std::string etatJoueur() const;

    unsigned int calculerScoreMax() const;

    const char* spriteJoueur() const;

    void afficherCarte();
    void afficherItems();
    void afficherEnnemis();
    void afficherFeux();
    void afficherJoueur();

    void afficher();
    void boucle();
};

#endif