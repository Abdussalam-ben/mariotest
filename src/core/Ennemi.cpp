#include "Ennemi.h"
#include "Constantes.h"
#include <iostream>
#include <cassert>

using namespace std;

Ennemi::Ennemi(const Vec2& p, const TypeEnnemi t)
    : type(t), etat(EtatEnnemi::EnVie), mouv(EtatMouvEnnemi::etat1), dtMouv(0) {
    pos = p;
    v = Vec2(-V_ENNEMI_X, 0.f);
    l = LARGEUR_ENNEMI;
    h = HAUTEUR_ENNEMI;
    dir = Direction::Gauche;
}

TypeEnnemi Ennemi::getType() const {
    return type;
}

EtatMouvEnnemi Ennemi::getMouv() const {
    return mouv;
}

EtatEnnemi Ennemi::getEtat() const {
    return etat;
}

bool Ennemi::estEnVie() const {
    return etat == EtatEnnemi::EnVie;
}

void Ennemi::mettreAJourMouvement() {
    if (mouv == EtatMouvEnnemi::etat1)
        mouv = EtatMouvEnnemi::etat2;
    else
        mouv = EtatMouvEnnemi::etat1;
}

void Ennemi::eliminer() {
    assert(etat == EtatEnnemi::EnVie);
    etat = EtatEnnemi::Mort;
}

unsigned int Ennemi::getDtDepuisMajMouvement() const {
    return dtMouv;
}

void Ennemi::mettreAJourDtDepuisMajMouvement(const unsigned int nDt) {
    dtMouv = nDt;
}

void Ennemi::testRegression() {
    cout << "=== Test Ennemi ===" << endl;

    Ennemi e(Vec2(10.f, 20.f), TypeEnnemi::Goomba);
    assert(e.getPos() == Vec2(10.f, 20.f));
    assert(e.getType() == TypeEnnemi::Goomba);
    assert(e.estEnVie());

    assert(e.getMouv() == EtatMouvEnnemi::etat1);
    e.mettreAJourMouvement();
    assert(e.getMouv() == EtatMouvEnnemi::etat2);

    e.mettreAJourDtDepuisMajMouvement(50);
    assert(e.getDtDepuisMajMouvement() == 50);

    e.eliminer();
    assert(!e.estEnVie());
    assert(e.getEtat() == EtatEnnemi::Mort);

    cout << "Tous les tests de Ennemi sont valides." << endl;
}
