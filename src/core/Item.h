#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"
#include "Constantes.h"

/**
 * @brief Représente les types d'objets ramassables.
 */
enum class TypeItem {
    piece,      ///< Pièce.
    champignon, ///< Champignon.
    fleur       ///< Fleur de feu.
};

/**
 * @brief Représente l'état d'un item.
 */
enum class EtatItem {
    disponible, ///< Item visible et ramassable.
    pris        ///< Item déjà pris.
};

/**
 * @brief Représente un item du jeu.
 */
class Item : public Entity {
private:
    TypeItem type; ///< Type de l'item.
    EtatItem etat; ///< État courant.

public:
    /**
     * @brief Empêche la création sans paramètres.
     */
    Item() = delete;

    /**
     * @brief Construit un item.
     * @param p Position initiale.
     * @param t Type de l'item.
     */
    Item(const Vec2& p, const TypeItem t);

    /**
     * @brief Retourne le type.
     * @return Type de l'item.
     */
    TypeItem getType() const;

    /**
     * @brief Retourne l'état.
     * @return État de l'item.
     */
    EtatItem getEtat() const;

    /**
     * @brief Indique si l'item est disponible.
     * @return true si l'item peut être pris.
     */
    bool estDisponible() const;

    /**
     * @brief Marque l'item comme pris.
     */
    void prendre();

    /**
     * @brief Lance les tests de régression.
     */
    static void testRegression();
};

#endif
