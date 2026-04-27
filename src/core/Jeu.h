#ifndef JEU_H
#define JEU_H

#include "Niveau.h"
#include "Joueur.h"
#include "EntreeJoueur.h"
#include "Ennemi.h"
#include "Item.h"
#include "PlateformeMobile.h"
#include "Tuile.h"
#include "Feu.h"

#include <vector>

/**
 * @brief Etat actuel de la partie.
 */
enum class EtatPartie
{
    enCours,
    gagnee,
    perdue
};

/**
 * @brief Gère la logique principale du jeu.
 */
class Jeu
{
private:
    Niveau& niv;
    Joueur& joueur;

    std::vector<Ennemi> ens;
    std::vector<Item> items;
    std::vector<PlateformeMobile> plats;
    std::vector<Tuile> myst;
    std::vector<Feu> boules;

    float temps;

    unsigned int score;
    unsigned int pieces;

    EtatPartie etat;

public:
    Jeu(Niveau& n, Joueur& j);

    void reinit();

    void maj(const EntreeJoueur& in, const float dt);

    float getTemps() const;

    unsigned int getScore() const;
    unsigned int getPieces() const;

    EtatPartie getEtat() const;

    const std::vector<Ennemi>& getEnnemis() const;
    const std::vector<Item>& getItems() const;
    const std::vector<Feu>& getBoules() const;

    static void testRegression();
};

#endif