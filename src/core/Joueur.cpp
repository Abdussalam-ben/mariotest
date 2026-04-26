#include "Joueur.h"
#include "Constantes.h"

#include <iostream>
#include <cassert>

using namespace std;

Joueur::Joueur(const Vec2& p)
    : type(TypeJoueur::petit),
      etat(EtatJoueur::EnVie),
      mouv(EtatMouvJoueur::rien),
      vies(NB_VIES_JOUEUR),
      perso(Personnage::mario),
      tempsEtoile(0.f),
      tempsProtection(0.f)
{
    pos = p;
    v = Vec2(0.f, 0.f);
    l = LARGEUR_JOUEUR;
    h = HAUTEUR_JOUEUR;
    dir = Direction::Droite;
}

void Joueur::MajMouv()
{
    if (v.x == 0.f)
    {
        mouv = EtatMouvJoueur::rien;
    }
    else if (mouv == EtatMouvJoueur::rien || mouv == EtatMouvJoueur::etat3)
    {
        mouv = EtatMouvJoueur::etat1;
    }
    else if (mouv == EtatMouvJoueur::etat1)
    {
        mouv = EtatMouvJoueur::etat2;
    }
    else
    {
        mouv = EtatMouvJoueur::etat3;
    }
}

TypeJoueur Joueur::getType() const
{
    return type;
}

EtatJoueur Joueur::getEtat() const
{
    return etat;
}

EtatMouvJoueur Joueur::getMouv() const
{
    return mouv;
}

unsigned int Joueur::getVies() const
{
    return vies;
}

Personnage Joueur::getPersonnage() const
{
    return perso;
}

float Joueur::getTempsEtoile() const
{
    return tempsEtoile;
}

void Joueur::setPersonnage(Personnage p)
{
    perso = p;
}

void Joueur::setType(TypeJoueur t)
{
    if (t == TypeJoueur::petit)
        devenirPetit();
    else if (t == TypeJoueur::grand)
        devenirGrand();
    else if (t == TypeJoueur::feu)
        devenirFeu();
}

void Joueur::devenirPetit()
{
    if (h > HAUTEUR_JOUEUR)
    {
        pos.y += h - HAUTEUR_JOUEUR;
    }

    type = TypeJoueur::petit;
    l = LARGEUR_JOUEUR;
    h = HAUTEUR_JOUEUR;
}

void Joueur::devenirGrand()
{
    if (type == TypeJoueur::petit)
    {
        pos.y -= HAUTEUR_JOUEUR_GRAND - HAUTEUR_JOUEUR;
    }

    type = TypeJoueur::grand;
    l = LARGEUR_JOUEUR;
    h = HAUTEUR_JOUEUR_GRAND;
}

void Joueur::devenirFeu()
{
    if (type == TypeJoueur::petit)
    {
        pos.y -= HAUTEUR_JOUEUR_GRAND - HAUTEUR_JOUEUR;
    }

    type = TypeJoueur::feu;
    l = LARGEUR_JOUEUR;
    h = HAUTEUR_JOUEUR_GRAND;
}

void Joueur::devenirEtoile(float duree)
{
    assert(duree > 0.f);

    /*
     * L'étoile ne change pas l'apparence du joueur.
     * Elle donne seulement une invincibilité temporaire.
     */
    tempsEtoile = duree;
}

void Joueur::majPouvoirs(float dt)
{
    assert(dt >= 0.f);

    if (tempsEtoile > 0.f)
    {
        tempsEtoile -= dt;

        if (tempsEtoile < 0.f)
            tempsEtoile = 0.f;
    }

    if (tempsProtection > 0.f)
    {
        tempsProtection -= dt;

        if (tempsProtection < 0.f)
            tempsProtection = 0.f;
    }
}

bool Joueur::estInvincible() const
{
    return tempsEtoile > 0.f;
}

bool Joueur::estProtege() const
{
    return tempsProtection > 0.f;
}

bool Joueur::peutTirerFeu() const
{
    return type == TypeJoueur::feu;
}

void Joueur::commencerProtection(float duree)
{
    assert(duree >= 0.f);
    tempsProtection = duree;
}

void Joueur::perdreVie()
{
    if (estInvincible() || estProtege())
    {
        return;
    }

    if (type == TypeJoueur::grand || type == TypeJoueur::feu)
    {
        devenirPetit();
        commencerProtection(1.5f);
        return;
    }

    perdreVieDirecte();
    commencerProtection(1.5f);
}

void Joueur::perdreVieDirecte()
{
    if (vies > 0)
    {
        vies--;
    }

    if (vies == 0)
    {
        etat = EtatJoueur::Mort;
    }
}

bool Joueur::estMort() const
{
    return vies == 0 || etat == EtatJoueur::Mort;
}

void Joueur::testRegression()
{
    cout << "=== Test Joueur ===" << endl;

    Joueur j(Vec2(8.f, 12.f));

    assert(j.getType() == TypeJoueur::petit);
    assert(j.getHauteur() == HAUTEUR_JOUEUR);

    j.devenirGrand();
    assert(j.getType() == TypeJoueur::grand);
    assert(j.getHauteur() == HAUTEUR_JOUEUR_GRAND);

    j.perdreVie();
    assert(j.getType() == TypeJoueur::petit);
    assert(j.getVies() == NB_VIES_JOUEUR);

    j.devenirFeu();
    assert(j.getType() == TypeJoueur::feu);
    assert(j.peutTirerFeu());

    j.devenirEtoile(5.f);
    assert(j.estInvincible());

    j.majPouvoirs(5.f);
    assert(!j.estInvincible());

    cout << "Tous les tests de Joueur sont valides." << endl;
}