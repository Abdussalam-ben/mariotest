#include "Tuile.h"
#include <iostream>
#include <cassert>

using namespace std;

Tuile::Tuile(const Vec2& p, const TypeItem& t)
    : pos(p), contenu(t), vide(false) {}

Vec2 Tuile::getPos() const {
    return pos;
}

TypeItem Tuile::getContenu() const {
    return contenu;
}

bool Tuile::estVide() const {
    return vide;
}

void Tuile::utiliser() {
    vide = true;
}

void Tuile::testRegression() {
    cout << "=== Test Tuile ===" << endl;

    Tuile t(Vec2(16.f, 32.f), TypeItem::champignon);
    assert(t.getPos() == Vec2(16.f, 32.f));
    assert(t.getContenu() == TypeItem::champignon);
    assert(!t.estVide());

    t.utiliser();
    assert(t.estVide());

    cout << "Tous les tests de Tuile sont valides." << endl;
}
