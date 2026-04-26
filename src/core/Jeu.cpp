#include "Jeu.h"
#include <iostream>
#include <cassert>

using namespace std;

/**
 * @brief Indique si une tuile bloque le joueur ou un ennemi.
 */
static bool estSolide(TypeTuile t) {
    return t == TypeTuile::sol ||
           t == TypeTuile::incassable ||
           t == TypeTuile::cassable ||
           t == TypeTuile::tuyau ||
           t == TypeTuile::mystere;
}

/**
 * @brief Vérifie si une position touche une tuile solide.
 */
static bool collisionTuile(const Niveau& n, const Vec2& p) {
    int tx = static_cast<int>(p.x / 16.f);
    int ty = static_cast<int>(p.y / 16.f);

    if (tx < 0 || ty < 0)
        return true;

    if (tx >= static_cast<int>(n.getDimX()) ||
        ty >= static_cast<int>(n.getDimY()))
        return false;

    return estSolide(n.getTuile(tx, ty));
}

/**
 * @brief Réinitialise les éléments du niveau après la perte d'une vie.
 */
static void reinitialiserObjets(Niveau& niv,
                                vector<Ennemi>& ens,
                                vector<Item>& items,
                                vector<PlateformeMobile>& plats,
                                vector<Tuile>& myst,
                                vector<Feu>& boules,
                                float& temps) {
    ens = niv.getEnnemisInit();
    items = niv.getItemsInit();
    plats = niv.getPlatsInit();
    myst = niv.getMystInit();
    boules.clear();
    temps = 300.f;
}

Jeu::Jeu(Niveau& n, Joueur& j)
    : niv(n), joueur(j), temps(300.f), score(0), etat(EtatPartie::enCours) {
    reinit();
}

void Jeu::reinit() {
    reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);

    score = 0;
    etat = EtatPartie::enCours;

    joueur.setPos(niv.getSpawn());
    joueur.setVit(Vec2(0.f, 0.f));
}

void Jeu::maj(const EntreeJoueur& in, const float dt) {
    assert(dt >= 0.f);

    if (etat != EtatPartie::enCours)
        return;

    Vec2 nv = joueur.getVit();
    Vec2 p = joueur.getPos();

    Vec2 dessous = p;
    dessous.y += 16.f;
    bool auSol = collisionTuile(niv, dessous);

    if (in.gauche) {
        nv.x = -150.f;
        joueur.setDir(Direction::Gauche);
    }
    else if (in.droite) {
        nv.x = 150.f;
        joueur.setDir(Direction::Droite);
    }
    else {
        nv.x = 0.f;
    }

    if (in.saut && auSol) {
        nv.y = -150.f;
    }
    else {
        nv.y += 80.f;
    }

    Vec2 anciennePos = p;

    p.x += nv.x * dt;

    if (p.x < 0.f) {
        p.x = 0.f;
    }

    if (collisionTuile(niv, p)) {
        p.x = anciennePos.x;
        nv.x = 0.f;
    }

    anciennePos = p;

    p.y += nv.y * dt;

    if (p.y < 0.f) {
        p.y = 0.f;
        nv.y = 0.f;
    }

    if (collisionTuile(niv, p)) {
        p.y = anciennePos.y;
        nv.y = 0.f;
    }

    if (p.y > static_cast<float>(niv.getDimY()) * 16.f) {
        joueur.perdreVie();

        if (joueur.estMort()) {
            etat = EtatPartie::perdue;
        }
        else {
            reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);
            p = niv.getSpawn();
            nv = Vec2(0.f, 0.f);
        }
    }

    joueur.setVit(nv);
    joueur.setPos(p);
    joueur.MajMouv();

    for (unsigned int i = 0; i < plats.size(); i++) {
        plats[i].maj(dt);
    }

    for (unsigned int i = 0; i < ens.size(); i++) {
        if (!ens[i].estEnVie())
            continue;

        Vec2 ep = ens[i].getPos();
        Vec2 ev = ens[i].getVit();
        Vec2 ancienneEp = ep;

        ep.x += ev.x * dt * 0.5f;

        if (collisionTuile(niv, ep)) {
            ep.x = ancienneEp.x;
            ev.x = -ev.x;
        }

        ens[i].setPos(ep);
        ens[i].setVit(ev);

        float dxEnnemi = joueur.getPos().x - ens[i].getPos().x;
        float dyEnnemi = joueur.getPos().y - ens[i].getPos().y;

        if (dxEnnemi < 16.f && dxEnnemi > -16.f &&
            dyEnnemi < 16.f && dyEnnemi > -16.f) {

            bool toucheParDessus = joueur.getPos().y < ens[i].getPos().y &&
                                   joueur.getVit().y > 0.f;

            if (toucheParDessus) {
                ens[i].eliminer();
                score += 100;

                Vec2 rebond = joueur.getVit();
                rebond.y = -80.f;
                joueur.setVit(rebond);
            }
            else {
                joueur.perdreVie();

                if (joueur.estMort()) {
                    etat = EtatPartie::perdue;
                }
                else {
                    reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);
                    joueur.setPos(niv.getSpawn());
                    joueur.setVit(Vec2(0.f, 0.f));
                }
            }
        }
    }

    if (in.tir) {
        boules.push_back(Feu(joueur.getPos(), joueur.getDir()));
    }

    for (unsigned int i = 0; i < boules.size(); i++) {
        boules[i].majTemps(dt);
    }

    for (unsigned int i = 0; i < items.size(); i++) {
        if (items[i].estDisponible()) {
            float dx = joueur.getPos().x - items[i].getPos().x;
            float dy = joueur.getPos().y - items[i].getPos().y;

            if (dx < 16.f && dx > -16.f &&
                dy < 16.f && dy > -16.f) {
                items[i].prendre();
                score += 10;
            }
        }
    }

    temps -= dt;

    if (temps <= 0.f) {
        temps = 0.f;
        etat = EtatPartie::perdue;
    }

    float dxFin = joueur.getPos().x - niv.getFin().x;
    float dyFin = joueur.getPos().y - niv.getFin().y;

    if (dxFin < 16.f && dxFin > -16.f &&
        dyFin < 16.f && dyFin > -16.f) {
        etat = EtatPartie::gagnee;
    }
}

float Jeu::getTemps() const {
    return temps;
}

unsigned int Jeu::getScore() const {
    return score;
}

EtatPartie Jeu::getEtat() const {
    return etat;
}

const vector<Ennemi>& Jeu::getEnnemis() const {
    return ens;
}

const vector<Item>& Jeu::getItems() const {
    return items;
}

const vector<Feu>& Jeu::getBoules() const {
    return boules;
}

void Jeu::testRegression() {
    cout << "=== Test Jeu ===" << endl;

    Niveau n(1, 10, 5);
    n.setSpawn(Vec2(0.f, 0.f));
    n.setFin(Vec2(100.f, 48.f));
    n.ajouteItem(Item(Vec2(10.f, 0.f), TypeItem::piece));

    Joueur j(Vec2(0.f, 0.f));
    Jeu g(n, j);

    assert(g.getTemps() == 300.f);
    assert(g.getScore() == 0);
    assert(g.getEtat() == EtatPartie::enCours);
    assert(j.getVies() == 3);

    EntreeJoueur in;
    in.droite = true;
    g.maj(in, 0.1f);
    assert(j.getPos().x >= 0.f);

    EntreeJoueur saut;
    saut.saut = true;
    g.maj(saut, 0.1f);
    assert(j.getPos().y >= 0.f);

    EntreeJoueur tir;
    tir.tir = true;
    g.maj(tir, 0.1f);
    assert(g.getBoules().size() == 1);

    cout << "Tous les tests de Jeu sont valides." << endl;
}

