#ifndef JEU_H
#define JEU_H

#include <vector>
#include "Vec2.h"
#include "Entity.h"
#include "Joueur.h"
#include "EntreeJoueur.h"
#include "Ennemi.h"
#include "Item.h"
#include "PlateformeMobile.h"
#include "Feu.h"
#include "Tuile.h"
#include "Niveau.h"

/**
 * @brief Représente l'état global d'une partie.
 */
enum class EtatPartie {
    enCours, ///< Partie en cours.
    gagnee,  ///< Partie gagnée.
    perdue   ///< Partie perdue.
};

/**
 * @brief Représente une partie en cours.
 */
class Jeu {
private:
    Niveau& niv; ///< Niveau utilisé.
    Joueur& joueur; ///< Joueur utilisé.
    std::vector<Ennemi> ens; ///< Ennemis actifs.
    std::vector<Item> items; ///< Items actifs.
    std::vector<PlateformeMobile> plats; ///< Plateformes actives.
    std::vector<Feu> boules; ///< Boules de feu actives.
    std::vector<Tuile> myst; ///< Tuiles mystère actives.
    float temps; ///< Temps restant.
    unsigned int score; ///< Score courant.
    EtatPartie etat; ///< État de la partie.

public:
    /**
     * @brief Construit une partie.
     * @param n Niveau.
     * @param j Joueur.
     */
    Jeu(Niveau& n, Joueur& j);

    /**
     * @brief Réinitialise la partie.
     */
    void reinit();

    /**
     * @brief Met à jour la partie.
     * @param in Entrée utilisateur.
     * @param dt Pas de temps.
     */
    void maj(const EntreeJoueur& in, const float dt);

    /**
     * @brief Retourne le temps restant.
     * @return Temps restant.
     */
    float getTemps() const;

    /**
     * @brief Retourne le score.
     * @return Score courant.
     */
    unsigned int getScore() const;

    /**
     * @brief Retourne l'état de la partie.
     * @return État global.
     */
    EtatPartie getEtat() const;

    /**
     * @brief Retourne les ennemis actifs.
     * @return Référence constante vers les ennemis.
     */
    const std::vector<Ennemi>& getEnnemis() const;

    /**
     * @brief Retourne les items actifs.
     * @return Référence constante vers les items.
     */
    const std::vector<Item>& getItems() const;

    /**
     * @brief Retourne les boules de feu actives.
     * @return Référence constante vers les projectiles.
     */
    const std::vector<Feu>& getBoules() const;

    /**
     * @brief Lance les tests de régression.
     */
    static void testRegression();
};

#endif
