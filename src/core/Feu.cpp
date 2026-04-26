#include "Feu.h"
#include <iostream>
#include <cassert>

using namespace std;

Feu::Feu(const Vec2& p, const Direction& d)
    : temps(3.f), etat(EtatBouleFeu::active) {
    pos = p;
    dir = d;
    l = 8.f;
    h = 8.f;

    if (d == Direction::Droite)
        v = Vec2(5.f, 0.f);
    else
        v = Vec2(-5.f, 0.f);
}

float Feu::getTemps() const {
    return temps;
}

EtatBouleFeu Feu::getEtat() const {
    return etat;
}

bool Feu::estActive() const {
    return etat == EtatBouleFeu::active;
}

void Feu::majTemps(const float dt) {
    assert(dt >= 0.f);

    if (etat == EtatBouleFeu::eteinte)
        return;

    temps -= dt;
    if (temps <= 0.f) {
        temps = 0.f;
        etat = EtatBouleFeu::eteinte;
    }
}

void Feu::eteindre() {
    etat = EtatBouleFeu::eteinte;
    temps = 0.f;
}

void Feu::testRegression() {
    cout << "=== Test Feu ===" << endl;

    Feu b(Vec2(3.f, 4.f), Direction::Droite);
    assert(b.getPos() == Vec2(3.f, 4.f));
    assert(b.estActive());

    b.majTemps(1.f);
    assert(b.getTemps() == 2.f);

    b.majTemps(2.f);
    assert(!b.estActive());
    assert(b.getEtat() == EtatBouleFeu::eteinte);

    cout << "Tous les tests de Feu sont valides." << endl;
}
