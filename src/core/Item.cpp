#include "Item.h"

#include <iostream>
#include <cassert>

using namespace std;

Item::Item(const Vec2& p, const TypeItem t)
    : type(t),
      etat(EtatItem::disponible)
{
    pos = p;
    v = Vec2(0.f, 0.f);
    dir = Direction::Gauche;

    if (t == TypeItem::piece)
    {
        l = 8.f;
        h = 16.f;
    }
    else
    {
        l = 16.f;
        h = 16.f;
    }
}

TypeItem Item::getType() const
{
    return type;
}

EtatItem Item::getEtat() const
{
    return etat;
}

bool Item::estDisponible() const
{
    return etat == EtatItem::disponible;
}

void Item::prendre()
{
    assert(etat == EtatItem::disponible);
    etat = EtatItem::pris;
}

void Item::testRegression()
{
    cout << "=== Test Item ===" << endl;

    Item i1(Vec2(10.f, 20.f), TypeItem::piece);
    assert(i1.getType() == TypeItem::piece);
    assert(i1.estDisponible());

    i1.prendre();
    assert(!i1.estDisponible());

    Item i2(Vec2(0.f, 0.f), TypeItem::champignon);
    assert(i2.getLargeur() == 16.f);
    assert(i2.getHauteur() == 16.f);

    Item i3(Vec2(0.f, 0.f), TypeItem::fleur);
    assert(i3.getType() == TypeItem::fleur);

    Item i4(Vec2(0.f, 0.f), TypeItem::etoile);
    assert(i4.getType() == TypeItem::etoile);

    cout << "Tous les tests de Item sont valides." << endl;
}