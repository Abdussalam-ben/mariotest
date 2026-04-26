#include "Joueur.h"
#include "Constantes.h"
#include <iostream>
#include <cassert>

using namespace std;

Joueur::Joueur(const Vec2& p)
    : type(TypeJoueur::petit), etat(EtatJoueur::EnVie),
      mouv(EtatMouvJoueur::rien), vies(NB_VIES_JOUEUR),
      perso(Personnage::mario) {
    pos = p;
    v = Vec2(0.f, 0.f);
    l = LARGEUR_JOUEUR;
    h = HAUTEUR_JOUEUR;
    dir = Direction::Droite;
}

void Joueur::MajMouv() {
    if (v.x == 0.f) {
        mouv = EtatMouvJoueur::rien;
    } else if (mouv == EtatMouvJoueur::rien || mouv == EtatMouvJoueur::etat3) {
        mouv = EtatMouvJoueur::etat1;
    } else if (mouv == EtatMouvJoueur::etat1) {
        mouv = EtatMouvJoueur::etat2;
    } else {
        mouv = EtatMouvJoueur::etat3;
    }
}

TypeJoueur Joueur::getType() const {
    return type;
}

EtatJoueur Joueur::getEtat() const {
    return etat;
}

EtatMouvJoueur Joueur::getMouv() const {
    return mouv;
}

unsigned int Joueur::getVies() const {
    return vies;
}

void Joueur::perdreVie() {
    if (vies > 0) {
        vies--;
    }

    if (vies == 0) {
        etat = EtatJoueur::Mort;
    }
}

bool Joueur::estMort() const {
    return vies == 0 || etat == EtatJoueur::Mort;
}

void Joueur::testRegression() {
    cout << "=== Test Joueur ===" << endl;

    Joueur j(Vec2(8.f, 12.f));
    assert(j.getPos() == Vec2(8.f, 12.f));
    assert(j.getType() == TypeJoueur::petit);
    assert(j.getEtat() == EtatJoueur::EnVie);
    assert(j.getVies() == NB_VIES_JOUEUR);
    assert(j.getMouv() == EtatMouvJoueur::rien);

    j.setVit(Vec2(3.f, 0.f));
    j.MajMouv();
    assert(j.getMouv() == EtatMouvJoueur::etat1);

    j.MajMouv();
    assert(j.getMouv() == EtatMouvJoueur::etat2);

    j.setVit(Vec2(0.f, 0.f));
    j.MajMouv();
    assert(j.getMouv() == EtatMouvJoueur::rien);

    cout << "Tous les tests de Joueur sont valides." << endl;
}
