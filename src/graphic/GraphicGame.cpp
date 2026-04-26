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

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

/**
 * @brief Convertit une position de grille en position pixel.
 */
static Vec2 posGrille(const unsigned int x, const unsigned int y) {
    return Vec2(static_cast<float>(x) * 16.f, static_cast<float>(y) * 16.f);
}

/**
 * @brief Charge un niveau depuis un fichier.
 */
static Niveau chargerNiveau(const string& chemin) {
    ifstream fichier(chemin.c_str());

    if (!fichier.is_open()) {
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

    for (unsigned int y = 0; y < hauteur; y++) {
        getline(fichier, ligne);

        for (unsigned int x = 0; x < largeur && x < ligne.size(); x++) {
            char c = ligne[x];

            if (c == '#')
                n.setTuile(x, y, TypeTuile::incassable);
            else if (c == 'B')
                n.setTuile(x, y, TypeTuile::cassable);
            else if (c == 'T')
                n.setTuile(x, y, TypeTuile::tuyau);
            else if (c == 'S')
                n.setSpawn(posGrille(x, y));
            else if (c == 'F')
                n.setFin(posGrille(x, y));
            else if (c == 'E')
                n.ajouteEnnemi(Ennemi(posGrille(x, y), TypeEnnemi::Goomba));
            else if (c == 'C')
                n.ajouteItem(Item(posGrille(x, y), TypeItem::piece));
            else if (c == 'M')
                n.ajouteItem(Item(posGrille(x, y), TypeItem::champignon));
            else if (c == 'L')
                n.ajouteItem(Item(posGrille(x, y), TypeItem::fleur));
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
static string cheminNiveau(const unsigned int id) {
    if (id == 0) return "assets/niveaux/level0.txt";
    if (id == 1) return "assets/niveaux/level1.txt";
    if (id == 2) return "assets/niveaux/level2.txt";
    if (id == 3) return "assets/niveaux/level3.txt";
    if (id == 4) return "assets/niveaux/level4_difficile.txt";
    if (id == 5) return "assets/niveaux/level5_long.txt";

    return "assets/niveaux/level0.txt";
}

/**
 * @brief Point d'entrée du mode graphique.
 */
int main() {
    cout << "Choisis un niveau graphique : ";
    unsigned int id;
    cin >> id;

    Niveau niveau = chargerNiveau(cheminNiveau(id));
    Joueur joueur(niveau.getSpawn());
    Jeu jeu(niveau, joueur);

    SDLContext contexte;
    TextureManager textures(contexte.getRenderer());
    GameGraphicMode mode(contexte, textures);

    mode.setJeu(jeu, niveau, joueur);
    mode.boucle();

    return 0;
}
