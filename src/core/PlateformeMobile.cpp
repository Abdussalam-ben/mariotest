#include "PlateformeMobile.h"
#include <iostream>
#include <cassert>

using namespace std;

PlateformeMobile::PlateformeMobile(const Vec2& p, const float a, const AxePlateforme& ax)
    : dep(p), amp(a), axe(ax), aller(true) {
    assert(a >= 0.f);

    pos = p;
    v = Vec2(0.f, 0.f);
    l = 32.f;
    h = 8.f;
    dir = Direction::Droite;
}

void PlateformeMobile::maj(const float dt) {
    assert(dt >= 0.f);

    float pas = 5.f * dt;

    if (axe == AxePlateforme::horizontal) {
        if (aller) pos.x += pas;
        else pos.x -= pas;

        if (pos.x >= dep.x + amp) {
            pos.x = dep.x + amp;
            aller = false;
        }
        if (pos.x <= dep.x) {
            pos.x = dep.x;
            aller = true;
        }
    }
    else {
        if (aller) pos.y += pas;
        else pos.y -= pas;

        if (pos.y >= dep.y + amp) {
            pos.y = dep.y + amp;
            aller = false;
        }
        if (pos.y <= dep.y) {
            pos.y = dep.y;
            aller = true;
        }
    }
}

Vec2 PlateformeMobile::getDep() const {
    return dep;
}

float PlateformeMobile::getAmp() const {
    return amp;
}

AxePlateforme PlateformeMobile::getAxe() const {
    return axe;
}

void PlateformeMobile::testRegression() {
    cout << "=== Test PlateformeMobile ===" << endl;

    PlateformeMobile p(Vec2(0.f, 0.f), 10.f, AxePlateforme::horizontal);
    p.maj(1.f);
    assert(p.getPos().x == 5.f);

    p.maj(1.f);
    assert(p.getPos().x == 10.f);

    p.maj(1.f);
    assert(p.getPos().x == 5.f);

    cout << "Tous les tests de PlateformeMobile sont valides." << endl;
}
