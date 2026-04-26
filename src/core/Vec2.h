#ifndef VEC2_H
#define VEC2_H

/**
 * @brief Représente un vecteur 2D ou une position en deux dimensions.
 */

struct Vec2
{
    float x; ///< Coordonnée horizontale.
    float y; ///< Coordonnée verticale.

    
    /**
     * @brief Construit un vecteur nul.
     *
     * Initialise les coordonnées à 0.
     */

    Vec2();


    /**
     * @brief Construit un vecteur à partir de deux coordonnées.
     * @param x2 Valeur de la coordonnée horizontale.
     * @param y2 Valeur de la coordonnée verticale.
     */

    Vec2(const float x2, const float y2);

        /**
     * @brief Compare le vecteur courant avec un autre vecteur.
     * @param v Vecteur à comparer.
     * @return true si les deux vecteurs sont égaux, false sinon.
     */

    bool operator==(const Vec2& v ) const;


    /**
     * @brief Ajoute un vecteur au vecteur courant.
     * @param v Vecteur à ajouter.
     * @return Une référence vers le vecteur modifié.
     */

    Vec2& operator+=(const Vec2& v);

/**
 * @brief Lance les tests de regression de la structure Vec2.
 *
 * Cette fonction verifie le bon fonctionnement des constructeurs
 * et des operateurs de la structure Vec2.
 */
    static void testRegression();
    
    
};

#endif
