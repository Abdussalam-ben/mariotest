#include "Entity.h"
#include <iostream>
#include <cassert>

using namespace std;

Entity::Entity() : pos(), v(), l(0.f), h(0.f), dir(Direction::Droite) {}

Entity::~Entity() {}

Vec2 Entity::getPos() const {
    return pos;
}

Vec2 Entity::getVit() const {
    return v;
}

Direction Entity::getDir() const {
    return dir;
}

float Entity::getLargeur() const {
    return l;
}

float Entity::getHauteur() const {
    return h;
}

void Entity::updateVit(const Vec2& dv) {
    v += dv;
}

void Entity::setPos(const Vec2& p) {
    pos = p;
}

void Entity::setVit(const Vec2& nv) {
    v = nv;
}

void Entity::setDir(const Direction& d) {
    dir = d;
}

void Entity::setLargeur(const float nl) {
    assert(nl >= 0.f);
    l = nl;
}

void Entity::setHauteur(const float nh) {
    assert(nh >= 0.f);
    h = nh;
}

void Entity::testRegression() {
    cout << "=== Test Entity ===" << endl;

    Entity e;
    assert(e.getPos() == Vec2(0.f, 0.f));
    assert(e.getVit() == Vec2(0.f, 0.f));
    assert(e.getDir() == Direction::Droite);

    e.setPos(Vec2(4.f, 6.f));
    assert(e.getPos() == Vec2(4.f, 6.f));

    e.setVit(Vec2(1.f, 2.f));
    e.updateVit(Vec2(3.f, -1.f));
    assert(e.getVit() == Vec2(4.f, 1.f));

    e.setDir(Direction::Gauche);
    assert(e.getDir() == Direction::Gauche);

    e.setLargeur(16.f);
    e.setHauteur(32.f);
    assert(e.getLargeur() == 16.f);
    assert(e.getHauteur() == 32.f);

    cout << "Tous les tests de Entity sont valides." << endl;
}
