#include "Niveau.h"
#include <iostream>
#include <cassert>

using namespace std;

Niveau::Niveau() : id(0), dimx(0), dimy(0), spawn(), fin() {}

Niveau::Niveau(const unsigned int i, const unsigned int nx, const unsigned int ny)
    : id(i), dimx(nx), dimy(ny), spawn(), fin() {
    assert(nx > 0);
    assert(ny > 0);
    tuiles.resize(nx * ny, TypeTuile::vide);
    tuilesInit = tuiles;
}

unsigned int Niveau::getId() const {
    return id;
}

unsigned int Niveau::getDimX() const {
    return dimx;
}

unsigned int Niveau::getDimY() const {
    return dimy;
}

Vec2 Niveau::getSpawn() const {
    return spawn;
}

Vec2 Niveau::getFin() const {
    return fin;
}

void Niveau::setSpawn(const Vec2& p) {
    spawn = p;
}

void Niveau::setFin(const Vec2& p) {
    fin = p;
}

TypeTuile Niveau::getTuile(const unsigned int x, const unsigned int y) const {
    assert(x < dimx);
    assert(y < dimy);
    return tuiles[y * dimx + x];
}

void Niveau::setTuile(const unsigned int x, const unsigned int y, const TypeTuile& t) {
    assert(x < dimx);
    assert(y < dimy);
    tuiles[y * dimx + x] = t;
}

void Niveau::reinit() {
    tuiles = tuilesInit;
}

const vector<Ennemi>& Niveau::getEnnemisInit() const {
    return ensInit;
}

const vector<Item>& Niveau::getItemsInit() const {
    return itemsInit;
}

const vector<PlateformeMobile>& Niveau::getPlatsInit() const {
    return platsInit;
}

const vector<Tuile>& Niveau::getMystInit() const {
    return mystInit;
}

void Niveau::ajouteEnnemi(const Ennemi& e) {
    ensInit.push_back(e);
}

void Niveau::ajouteItem(const Item& i) {
    itemsInit.push_back(i);
}

void Niveau::ajoutePlateforme(const PlateformeMobile& p) {
    platsInit.push_back(p);
}

void Niveau::ajouteMystery(const Tuile& t) {
    mystInit.push_back(t);
}

void Niveau::testRegression() {
    cout << "=== Test Niveau ===" << endl;

    Niveau n(1, 4, 3);
    assert(n.getId() == 1);
    assert(n.getDimX() == 4);
    assert(n.getDimY() == 3);

    n.setSpawn(Vec2(16.f, 32.f));
    n.setFin(Vec2(64.f, 32.f));
    assert(n.getSpawn() == Vec2(16.f, 32.f));
    assert(n.getFin() == Vec2(64.f, 32.f));

    n.setTuile(1, 1, TypeTuile::sol);
    assert(n.getTuile(1, 1) == TypeTuile::sol);

    n.reinit();
    assert(n.getTuile(1, 1) == TypeTuile::vide);

    n.ajouteEnnemi(Ennemi(Vec2(0.f, 0.f), TypeEnnemi::Goomba));
    n.ajouteItem(Item(Vec2(10.f, 0.f), TypeItem::piece));
    assert(n.getEnnemisInit().size() == 1);
    assert(n.getItemsInit().size() == 1);

    cout << "Tous les tests de Niveau sont valides." << endl;
}
