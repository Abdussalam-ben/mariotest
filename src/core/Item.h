#ifndef ITEM_H
#define ITEM_H

#include "Entity.h"
#include "Constantes.h"

/**
 * @brief Représente les types d'objets ramassables.
 */
enum class TypeItem
{
    piece,
    champignon,
    fleur,
    etoile
};

/**
 * @brief Représente l'état d'un item.
 */
enum class EtatItem
{
    disponible,
    pris
};

/**
 * @brief Représente un item du jeu.
 */
class Item : public Entity
{
private:
    TypeItem type;
    EtatItem etat;

public:
    Item() = delete;

    Item(const Vec2& p, const TypeItem t);

    TypeItem getType() const;
    EtatItem getEtat() const;

    bool estDisponible() const;
    void prendre();

    static void testRegression();
};

#endif