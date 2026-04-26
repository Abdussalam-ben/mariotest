#include "Hitbox.h"
#include <iostream>
#include <cassert>

using namespace std;

Hitbox::Hitbox() : x(0.f), y(0.f), largeur(0.f), hauteur(0.f) {}

Hitbox::Hitbox(const float x2, const float y2, const float l2, const float h2) {
    assert(l2 >= 0.f);
    assert(h2 >= 0.f);

    x = x2;
    y = y2;
    largeur = l2;
    hauteur = h2;
}

bool Hitbox::collision(const Hitbox& col) const {
    return !(x + largeur <= col.x ||
             y + hauteur <= col.y ||
             col.x + col.largeur <= x ||
             col.y + col.hauteur <= y);
}

void Hitbox::testRegression() {
    cout << "=== Test Hitbox ===" << endl;

    Hitbox h1(0.f, 0.f, 10.f, 10.f);
    Hitbox h2(5.f, 5.f, 10.f, 10.f);
    Hitbox h3(20.f, 20.f, 5.f, 5.f);

    assert(h1.collision(h2));
    assert(!h1.collision(h3));

    cout << "Tous les tests de Hitbox sont valides." << endl;
}
