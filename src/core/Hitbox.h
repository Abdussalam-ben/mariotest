#ifndef HITBOX_H
#define HITBOX_H

/**
 * @brief Représente une boîte de collision rectangulaire.
 */
struct Hitbox
{
    float x; ///< Coordonnée horizontale du coin supérieur gauche.
    float y; ///< Coordonnée verticale du coin supérieur gauche.
    float largeur; ///< Largeur de la hitbox.
    float hauteur; ///< Hauteur de la hitbox.

    /**
     * @brief Construit une hitbox vide.
     */
    Hitbox();

    /**
     * @brief Construit une hitbox à partir de sa position et de sa taille.
     * @param x2 Position horizontale.
     * @param y2 Position verticale.
     * @param l2 Largeur.
     * @param h2 Hauteur.
     */
    Hitbox(const float x2, const float y2, const float l2, const float h2);

    /**
     * @brief Teste la collision avec une autre hitbox.
     * @param col Hitbox à tester.
     * @return true si les deux hitbox se touchent, false sinon.
     */
    bool collision(const Hitbox& col) const;

    /**
     * @brief Lance les tests de régression de Hitbox.
     */
    static void testRegression();
};

#endif
