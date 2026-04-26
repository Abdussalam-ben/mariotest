#include "ModeTexte.h"

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
 * @param x Colonne.
 * @param y Ligne.
 * @return Position en pixels.
 */
static Vec2 posGrille(const unsigned int x, const unsigned int y) {
    return Vec2((float)x * 16.f, (float)y * 16.f);
}

/**
 * @brief Lit le meilleur score d'un niveau.
 * @param id Identifiant du niveau.
 * @return Meilleur score lu, ou 0 si aucun fichier n'existe.
 */
static unsigned int lireMeilleurScore(const unsigned int id) {
    string chemin = "assets/score/score" + to_string(id) + ".txt";
    ifstream fichier(chemin.c_str());

    unsigned int score = 0;

    if (fichier.is_open()) {
        fichier >> score;
        fichier.close();
    }

    return score;
}

/**
 * @brief Sauvegarde le meilleur score d'un niveau.
 * @param id Identifiant du niveau.
 * @param score Score à sauvegarder.
 */
static void sauvegarderMeilleurScore(const unsigned int id, const unsigned int score) {
    string chemin = "assets/score/score" + to_string(id) + ".txt";
    ofstream fichier(chemin.c_str());

    if (fichier.is_open()) {
        fichier << score << endl;
        fichier.close();
    }
}

/**
 * @brief Charge un niveau depuis un fichier texte.
 * @param chemin Chemin du fichier.
 * @return Niveau chargé.
 */
static Niveau chargerNiveau(const string& chemin) {
    ifstream fichier(chemin.c_str());

    if (!fichier.is_open()) {
        cerr << "Erreur : impossible d'ouvrir le fichier " << chemin << endl;
        exit(1);
    }

    unsigned int id;
    unsigned int largeur;
    unsigned int hauteur;

    fichier >> id >> largeur >> hauteur;

    Niveau n(id, largeur, hauteur);

    string ligne;
    getline(fichier, ligne); // consomme la fin de la première ligne

    for (unsigned int y = 0; y < hauteur; y++) {
        getline(fichier, ligne);

        for (unsigned int x = 0; x < largeur && x < ligne.size(); x++) {
            char c = ligne[x];

            if (c == '#') {
                n.setTuile(x, y, TypeTuile::incassable);
            }
            else if (c == 'B') {
                n.setTuile(x, y, TypeTuile::cassable);
            }
            else if (c == 'T') {
                n.setTuile(x, y, TypeTuile::tuyau);
            }
            else if (c == 'S') {
                n.setSpawn(posGrille(x, y));
            }
            else if (c == 'F') {
                n.setFin(posGrille(x, y));
            }
            else if (c == 'E') {
                n.ajouteEnnemi(Ennemi(posGrille(x, y), TypeEnnemi::Goomba));
            }
            else if (c == 'C') {
                n.ajouteItem(Item(posGrille(x, y), TypeItem::piece));
            }
            else if (c == 'M') {
                n.ajouteItem(Item(posGrille(x, y), TypeItem::champignon));
            }
            else if (c == 'L') {
                n.ajouteItem(Item(posGrille(x, y), TypeItem::fleur));
            }
            else if (c == 'P') {
                n.ajoutePlateforme(PlateformeMobile(posGrille(x, y), 48.f, AxePlateforme::horizontal));
            }
            else {
                n.setTuile(x, y, TypeTuile::vide);
            }
        }
    }

    fichier.close();
    return n;
}

/**
 * @brief Demande à l'utilisateur quel niveau lancer.
 * @return Identifiant choisi.
 */
static unsigned int choisirNiveau() {
    unsigned int id;

    cout << "Choisis un niveau : ";
    cin >> id;

    return id;
}

/**
 * @brief Point d'entrée du jeu en mode texte.
 */
int main() {
    cout << "=== Super Mario - Mode texte ===" << endl;

    unsigned int id = choisirNiveau();
    string chemin = "assets/niveaux/level" + to_string(id) + ".txt";

    Niveau niveau = chargerNiveau(chemin);
    Joueur joueur(niveau.getSpawn());
    Jeu jeu(niveau, joueur);

    unsigned int meilleurScore = lireMeilleurScore(niveau.getId());

    ModeTexte mode(jeu, niveau, joueur);
    mode.boucle(meilleurScore);

    if (jeu.getScore() > meilleurScore) {
        sauvegarderMeilleurScore(niveau.getId(), jeu.getScore());
        cout << "Nouveau meilleur score : " << jeu.getScore() << endl;
    }
    else {
        cout << "Score final : " << jeu.getScore() << endl;
        cout << "Meilleur score conserve : " << meilleurScore << endl;
    }

    return 0;
}
