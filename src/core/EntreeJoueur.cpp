#include "EntreeJoueur.h"
#include <iostream>
#include <cassert>

using namespace std;

//EntreeJoueur::EntreeJoueur()
  //  : gauche(false), droite(false), saut(false), tir(false) {}

void EntreeJoueur::testRegression() {
    cout << "=== Test EntreeJoueur ===" << endl;

    EntreeJoueur input;

    assert(input.gauche == false);
    assert(input.droite == false);
    assert(input.saut == false);
    assert(input.tir == false);

    input.gauche = true;
    input.saut = true;

    assert(input.gauche == true);
    assert(input.droite == false);
    assert(input.saut == true);
    assert(input.tir == false);

    cout << "Tous les tests de EntreeJoueur sont valides." << endl;
}