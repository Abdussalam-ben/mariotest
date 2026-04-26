#include <iostream>
#include "Vec2.h"
#include <cassert>


using namespace std;

Vec2::Vec2() : x(0.f) , y(0.f) {}

Vec2::Vec2(const float x2, const float y2) : x(x2) , y(y2) {}

bool Vec2::operator==(const Vec2& v ) const {
    return x == v.x && y == v.y; }

Vec2& Vec2::operator+=(const Vec2& v) {
    x += v.x;
    y += v.y;
    return *this; }




    void Vec2::testRegression() {
    cout << "===== Debut des tests de regression de Vec2 =====" << endl;

    // Test 1 : verification du constructeur par defaut
    Vec2 v1;
    assert(v1.x == 0.f);
    assert(v1.y == 0.f);
    cout << "Test 1 valide : constructeur par defaut." << endl;

    // Test 2 : verification du constructeur avec parametres positifs
    Vec2 v2(4.f, 7.f);
    assert(v2.x == 4.f);
    assert(v2.y == 7.f);
    cout << "Test 2 valide : constructeur parametre avec valeurs positives." << endl;

    // Test 3 : verification du constructeur avec une valeur negative
    Vec2 v3(-2.f, 5.f);
    assert(v3.x == -2.f);
    assert(v3.y == 5.f);
    cout << "Test 3 valide : constructeur parametre avec valeur negative." << endl;

    // Test 4 : verification de operator== pour deux vecteurs egaux
    Vec2 a(1.f, 2.f);
    Vec2 b(1.f, 2.f);
    assert(a == b);
    cout << "Test 4 valide : operator== detecte correctement deux vecteurs egaux." << endl;

    // Test 5 : verification de operator== pour deux vecteurs differents
    Vec2 c(1.f, 2.f);
    Vec2 d(2.f, 2.f);
    assert(!(c == d));
    cout << "Test 5 valide : operator== detecte correctement une difference sur x." << endl;

    // Test 6 : verification de operator== pour une difference sur y
    Vec2 e(3.f, 4.f);
    Vec2 f(3.f, 8.f);
    assert(!(e == f));
    cout << "Test 6 valide : operator== detecte correctement une difference sur y." << endl;

    // Test 7 : verification de operator+=
    Vec2 g(2.f, 3.f);
    Vec2 h(5.f, -1.f);
    g += h;
    assert(g.x == 7.f);
    assert(g.y == 2.f);
    cout << "Test 7 valide : operator+= additionne correctement les composantes." << endl;

    // Test 8 : verification que operator+= modifie bien l'objet courant
    Vec2 i(10.f, 1.f);
    Vec2 j(-3.f, 6.f);
    Vec2* adresseAvant = &i;
    Vec2& ref = (i += j);
    assert(&ref == adresseAvant);
    assert(i.x == 7.f);
    assert(i.y == 7.f);
    cout << "Test 8 valide : operator+= renvoie bien une reference sur l'objet modifie." << endl;

    cout << "===== Fin des tests de regression de Vec2 : tout est valide =====" << endl;
}





