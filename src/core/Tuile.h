#ifndef TUILE_H
#define TUILE_H

#include "Vec2.h"
#include "Item.h"

/**
 * @brief Représente une tuile mystere.
 */
class Tuile {
private:
    Vec2 pos; ///< Position de la tuile.
    TypeItem contenu; ///< Contenu de la tuile.
    bool vide; ///< Indique si la tuile a déjà été utilisée.

public:
    /**
     * @brief Empêche la création sans paramètres.
     */
    Tuile() = delete;

    /**
     * @brief Construit une tuile mystere.
     * @param p Position.
     * @param t Type d'item contenu.
     */
    Tuile(const Vec2& p, const TypeItem& t);

    /**
     * @brief Retourne la position.
     * @return Position de la tuile.
     */
    Vec2 getPos() const;

    /**
     * @brief Retourne le contenu.
     * @return Type de l'item contenu.
     */
    TypeItem getContenu() const;

    /**
     * @brief Indique si la tuile est vide.
     * @return true si elle a déjà été utilisée.
     */
    bool estVide() const;

    /**
     * @brief Active la tuile.
     */
    void utiliser();

    /**
     * @brief Lance les tests de régression.
     */
    static void testRegression();
};

#endif
