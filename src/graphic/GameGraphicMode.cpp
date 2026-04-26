#include "GameGraphicMode.h"
#include <iostream>
#include <vector>

using namespace std;

GameGraphicMode::GameGraphicMode(SDLContext& c, TextureManager& t)
    : contexte(&c), textures(&t), jeu(nullptr), niveau(nullptr),
      joueur(nullptr), actif(true), frame(0) {}

void GameGraphicMode::setJeu(Jeu& j, Niveau& n, Joueur& p) {
    jeu = &j;
    niveau = &n;
    joueur = &p;
}

EntreeJoueur GameGraphicMode::lireEntree() const {
    EntreeJoueur entree;

    const Uint8* clavier = SDL_GetKeyboardState(NULL);

    entree.gauche = clavier[SDL_SCANCODE_Q] || clavier[SDL_SCANCODE_LEFT];
    entree.droite = clavier[SDL_SCANCODE_D] || clavier[SDL_SCANCODE_RIGHT];
    entree.saut = clavier[SDL_SCANCODE_SPACE] || clavier[SDL_SCANCODE_Z] || clavier[SDL_SCANCODE_UP];
    entree.tir = clavier[SDL_SCANCODE_F];

    return entree;
}

void GameGraphicMode::afficherSprite(const char* nom, int x, int y, int l, int h, bool flip) {
    string chemin = string("assets/sprites/") + nom;
    SDL_Texture* tex = textures->getTexture(chemin);

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = l;
    dst.h = h;

    SDL_RendererFlip f = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    SDL_RenderCopyEx(contexte->getRenderer(), tex, NULL, &dst, 0.0, NULL, f);
}

const char* GameGraphicMode::spriteJoueur() const {
    if (joueur == nullptr)
        return "mario_base.png";

    if (joueur->getVit().y < 0.f) {
        if (joueur->getDir() == Direction::Gauche)
            return "mario_saut_gauche.png";
        return "mario_saut_droite.png";
    }

    if (joueur->getVit().x > 0.f) {
        unsigned int k = (frame / 8) % 3;
        if (k == 0) return "mario_droite_pas1.png";
        if (k == 1) return "mario_droite_pas2.png";
        return "mario_droite_pas3.png";
    }

    if (joueur->getVit().x < 0.f) {
        unsigned int k = (frame / 8) % 3;
        if (k == 0) return "mario_gauche_pas1.png";
        if (k == 1) return "mario_gauche_pas2.png";
        return "mario_gauche_pas3.png";
    }

    return "mario_base.png";
}

void GameGraphicMode::afficherCarte() {
    if (niveau == nullptr)
        return;

    for (unsigned int y = 0; y < niveau->getDimY(); y++) {
        for (unsigned int x = 0; x < niveau->getDimX(); x++) {
            TypeTuile t = niveau->getTuile(x, y);

            int px = static_cast<int>(x * 16);
            int py = static_cast<int>(y * 16);

            if (t == TypeTuile::sol || t == TypeTuile::incassable) {
                afficherSprite("bloc_dur.png", px, py, 16, 16);
            }
            else if (t == TypeTuile::cassable) {
                afficherSprite("bloc_cassable.png", px, py, 16, 16);
            }
            else if (t == TypeTuile::tuyau) {
                afficherSprite("tuyau.png", px, py, 16, 16);
            }
            else if (t == TypeTuile::mystere) {
                unsigned int k = (frame / 12) % 3;
                if (k == 0) afficherSprite("tuile_actif1.png", px, py, 16, 16);
                else if (k == 1) afficherSprite("tuile_actif2.png", px, py, 16, 16);
                else afficherSprite("tuile_actif3.png", px, py, 16, 16);
            }
        }
    }

    int fx = static_cast<int>(niveau->getFin().x);
    int fy = static_cast<int>(niveau->getFin().y);
    afficherSprite("tuile_vide.png", fx, fy, 16, 16);
}

void GameGraphicMode::afficherItems() {
    if (jeu == nullptr)
        return;

    const vector<Item>& items = jeu->getItems();

    for (unsigned int i = 0; i < items.size(); i++) {
        if (!items[i].estDisponible())
            continue;

        int x = static_cast<int>(items[i].getPos().x);
        int y = static_cast<int>(items[i].getPos().y);

        if (items[i].getType() == TypeItem::piece)
            afficherSprite("piece.png", x, y, 16, 16);
        else if (items[i].getType() == TypeItem::champignon)
            afficherSprite("champignon.png", x, y, 16, 16);
        else if (items[i].getType() == TypeItem::fleur)
            afficherSprite("fleur_feu.png", x, y, 16, 16);
    }
}

void GameGraphicMode::afficherEnnemis() {
    if (jeu == nullptr)
        return;

    const vector<Ennemi>& ens = jeu->getEnnemis();

    for (unsigned int i = 0; i < ens.size(); i++) {
        if (!ens[i].estEnVie())
            continue;

        int x = static_cast<int>(ens[i].getPos().x);
        int y = static_cast<int>(ens[i].getPos().y);

        if (ens[i].getType() == TypeEnnemi::Goomba) {
            if ((frame / 12) % 2 == 0)
                afficherSprite("goomba_pas1.png", x, y, 16, 16);
            else
                afficherSprite("goomba_pas2.png", x, y, 16, 16);
        }
        else {
            if (ens[i].getDir() == Direction::Gauche) {
                if ((frame / 12) % 2 == 0)
                    afficherSprite("koopa_gauche_pas1.png", x, y, 16, 24);
                else
                    afficherSprite("koopa_gauche_pas2.png", x, y, 16, 24);
            }
            else {
                if ((frame / 12) % 2 == 0)
                    afficherSprite("koopa_droite_pas1.png", x, y, 16, 24);
                else
                    afficherSprite("koopa_droite_pas2.png", x, y, 16, 24);
            }
        }
    }
}

void GameGraphicMode::afficherFeux() {
    if (jeu == nullptr)
        return;

    const vector<Feu>& feux = jeu->getBoules();

    for (unsigned int i = 0; i < feux.size(); i++) {
        if (feux[i].estActive()) {
            int x = static_cast<int>(feux[i].getPos().x);
            int y = static_cast<int>(feux[i].getPos().y);
            afficherSprite("fleur_feu.png", x, y, 12, 12);
        }
    }
}

void GameGraphicMode::afficherJoueur() {
    if (joueur == nullptr)
        return;

    int x = static_cast<int>(joueur->getPos().x);
    int y = static_cast<int>(joueur->getPos().y);

    afficherSprite(spriteJoueur(), x, y, 16, 16);
}

void GameGraphicMode::afficher() {
    SDL_Renderer* r = contexte->getRenderer();

    SDL_SetRenderDrawColor(r, 92, 148, 252, 255);
    SDL_RenderClear(r);

    afficherCarte();
    afficherItems();
    afficherEnnemis();
    afficherFeux();
    afficherJoueur();

    SDL_RenderPresent(r);
}

void GameGraphicMode::boucle() {
    actif = true;

    while (actif) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                actif = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                actif = false;
        }

        if (jeu != nullptr) {
            EntreeJoueur entree = lireEntree();
            jeu->maj(entree, 1.f / 60.f);
        }

        afficher();

        frame++;
        SDL_Delay(1000 / 60);
    }
}
