#ifndef ENTREEJOUEUR_H
#define ENTREEJOUEUR_H

/**
 * @brief Represente les commandes du joueur pendant une frame.
 */
struct EntreeJoueur
{
    bool gauche; ///< Déplacement vers la gauche.
    bool droite; ///< Déplacement vers la droite.
    bool saut;   ///< Commande de saut.
    bool tir;    ///< Commande de tir.

    /**
     * @brief Construit une entrée vide.
     */
    EntreeJoueur() : gauche(false), droite(false), saut(false), tir(false) {}

        /**
     * @brief Lance les tests de régression de UserInput.
     */
    static void testRegression();
};

#endif
