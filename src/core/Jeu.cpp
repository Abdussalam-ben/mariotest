#include "Jeu.h"
#include "Hitbox.h"
#include "Constantes.h"

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

static const float TAILLE_TUILE = 16.f;
static const float VITESSE_SAUT_JEU = 340.f;

/*
 * Le sprite du joueur est dessiné en 16x16,
 * mais sa hitbox est volontairement plus fine.
 * Cela permet de passer dans les espaces serrés.
 */
static const float HITBOX_JOUEUR_X = 3.f;
static const float HITBOX_JOUEUR_Y = 0.f;
static const float HITBOX_JOUEUR_L = 10.f;
static const float HITBOX_JOUEUR_H = 16.f;

/**
 * @brief Indique si une tuile bloque le joueur ou un ennemi.
 */
static bool estSolide(TypeTuile t)
{
    return t == TypeTuile::sol ||
           t == TypeTuile::incassable ||
           t == TypeTuile::cassable ||
           t == TypeTuile::tuyau ||
           t == TypeTuile::mystere;
}

/**
 * @brief Retourne true si la tuile donnée est solide.
 */
static bool tuileSolide(const Niveau& n, int x, int y)
{
    if (x < 0 || y < 0)
        return true;

    if (x >= static_cast<int>(n.getDimX()) ||
        y >= static_cast<int>(n.getDimY()))
        return false;

    return estSolide(n.getTuile(static_cast<unsigned int>(x),
                                static_cast<unsigned int>(y)));
}

/**
 * @brief Crée la hitbox du joueur.
 */
static Hitbox hitboxJoueur(const Vec2& p)
{
    return Hitbox(
        p.x + HITBOX_JOUEUR_X,
        p.y + HITBOX_JOUEUR_Y,
        HITBOX_JOUEUR_L,
        HITBOX_JOUEUR_H
    );
}

/**
 * @brief Crée une hitbox de 16x16 pour items, ennemis, arrivée.
 */
static Hitbox hitboxObjet16(const Vec2& p)
{
    return Hitbox(p.x, p.y, 16.f, 16.f);
}

/**
 * @brief Vérifie si une hitbox rectangulaire touche une tuile solide.
 */
static bool collisionTuileHitbox(const Niveau& n, const Hitbox& h)
{
    int gauche = static_cast<int>(h.x / TAILLE_TUILE);
    int droite = static_cast<int>((h.x + h.largeur - 1.f) / TAILLE_TUILE);
    int haut = static_cast<int>(h.y / TAILLE_TUILE);
    int bas = static_cast<int>((h.y + h.hauteur - 1.f) / TAILLE_TUILE);

    for (int y = haut; y <= bas; y++)
    {
        for (int x = gauche; x <= droite; x++)
        {
            if (tuileSolide(n, x, y))
                return true;
        }
    }

    return false;
}

/**
 * @brief Corrige la position si le joueur apparaît dans un bloc.
 */
static Vec2 sortirDuSol(const Niveau& n, Vec2 p)
{
    Hitbox h = hitboxJoueur(p);

    while (collisionTuileHitbox(n, h) && p.y > 0.f)
    {
        p.y -= 1.f;
        h = hitboxJoueur(p);
    }

    return p;
}

/**
 * @brief Collision horizontale avec correction exacte.
 */
static void corrigerCollisionHorizontale(const Niveau& n, Vec2& p, Vec2& v)
{
    Hitbox h = hitboxJoueur(p);

    if (!collisionTuileHitbox(n, h))
        return;

    int haut = static_cast<int>(h.y / TAILLE_TUILE);
    int bas = static_cast<int>((h.y + h.hauteur - 1.f) / TAILLE_TUILE);

    if (v.x > 0.f)
    {
        int droite = static_cast<int>((h.x + h.largeur - 1.f) / TAILLE_TUILE);

        for (int y = haut; y <= bas; y++)
        {
            if (tuileSolide(n, droite, y))
            {
                /*
                 * On replace p.x, pas h.x.
                 * Donc on retire aussi HITBOX_JOUEUR_X.
                 */
                p.x = static_cast<float>(droite) * TAILLE_TUILE
                      - HITBOX_JOUEUR_L
                      - HITBOX_JOUEUR_X;

                v.x = 0.f;
                return;
            }
        }
    }
    else if (v.x < 0.f)
    {
        int gauche = static_cast<int>(h.x / TAILLE_TUILE);

        for (int y = haut; y <= bas; y++)
        {
            if (tuileSolide(n, gauche, y))
            {
                /*
                 * Même logique : on replace la position du sprite,
                 * donc on retire l'offset horizontal de la hitbox.
                 */
                p.x = static_cast<float>(gauche + 1) * TAILLE_TUILE
                      - HITBOX_JOUEUR_X;

                v.x = 0.f;
                return;
            }
        }
    }
}

/**
 * @brief Collision verticale avec correction exacte.
 */
static void corrigerCollisionVerticale(const Niveau& n, Vec2& p, Vec2& v)
{
    Hitbox h = hitboxJoueur(p);

    if (!collisionTuileHitbox(n, h))
        return;

    int gauche = static_cast<int>(h.x / TAILLE_TUILE);
    int droite = static_cast<int>((h.x + h.largeur - 1.f) / TAILLE_TUILE);

    if (v.y > 0.f)
    {
        int bas = static_cast<int>((h.y + h.hauteur - 1.f) / TAILLE_TUILE);

        for (int x = gauche; x <= droite; x++)
        {
            if (tuileSolide(n, x, bas))
            {
                p.y = static_cast<float>(bas) * TAILLE_TUILE - h.hauteur;
                v.y = 0.f;
                return;
            }
        }
    }
    else if (v.y < 0.f)
    {
        int haut = static_cast<int>(h.y / TAILLE_TUILE);

        for (int x = gauche; x <= droite; x++)
        {
            if (tuileSolide(n, x, haut))
            {
                p.y = static_cast<float>(haut + 1) * TAILLE_TUILE;
                v.y = 0.f;
                return;
            }
        }
    }
}

/**
 * @brief Indique si le joueur est posé sur le sol.
 */
static bool joueurAuSol(const Niveau& n, const Vec2& p)
{
    Hitbox h = hitboxJoueur(p);
    h.y += 1.f;

    return collisionTuileHitbox(n, h);
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
                                float& temps)
{
    ens = niv.getEnnemisInit();
    items = niv.getItemsInit();
    plats = niv.getPlatsInit();
    myst = niv.getMystInit();
    boules.clear();
    temps = 300.f;
}

Jeu::Jeu(Niveau& n, Joueur& j)
    : niv(n),
      joueur(j),
      temps(300.f),
      score(0),
      etat(EtatPartie::enCours)
{
    reinit();
}

void Jeu::reinit()
{
    reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);

    score = 0;
    etat = EtatPartie::enCours;

    Vec2 spawn = sortirDuSol(niv, niv.getSpawn());

    joueur.setPos(spawn);
    joueur.setVit(Vec2(0.f, 0.f));
}

void Jeu::maj(const EntreeJoueur& in, const float dt)
{
    assert(dt >= 0.f);

    if (etat != EtatPartie::enCours)
        return;

    Vec2 v = joueur.getVit();
    Vec2 p = joueur.getPos();

    /*
     * Sécurité : si le joueur commence dans un bloc,
     * on le remonte immédiatement au-dessus.
     */
    p = sortirDuSol(niv, p);

    bool auSol = joueurAuSol(niv, p);

    if (in.gauche)
    {
        v.x = -V_MAX_JOUEUR_X;
        joueur.setDir(Direction::Gauche);
    }
    else if (in.droite)
    {
        v.x = V_MAX_JOUEUR_X;
        joueur.setDir(Direction::Droite);
    }
    else
    {
        v.x = 0.f;
    }

    if (in.saut && auSol)
    {
        v.y = -VITESSE_SAUT_JEU;
    }
    else
    {
        v.y += GRAVITE * dt;
    }

    /*
     * Déplacement horizontal.
     */
    p.x += v.x * dt;

    if (p.x < 0.f)
    {
        p.x = 0.f;
        v.x = 0.f;
    }

    corrigerCollisionHorizontale(niv, p, v);

    /*
     * Déplacement vertical.
     */
    p.y += v.y * dt;

    if (p.y < 0.f)
    {
        p.y = 0.f;
        v.y = 0.f;
    }

    corrigerCollisionVerticale(niv, p, v);

    /*
     * Sécurité finale : impossible de rester dans un bloc.
     */
    p = sortirDuSol(niv, p);

    if (p.y > static_cast<float>(niv.getDimY()) * TAILLE_TUILE)
    {
        joueur.perdreVie();

        if (joueur.estMort())
        {
            etat = EtatPartie::perdue;
        }
        else
        {
            reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);

            p = sortirDuSol(niv, niv.getSpawn());
            v = Vec2(0.f, 0.f);
        }
    }

    joueur.setVit(v);
    joueur.setPos(p);
    joueur.MajMouv();

    for (unsigned int i = 0; i < plats.size(); i++)
    {
        plats[i].maj(dt);
    }

    for (unsigned int i = 0; i < ens.size(); i++)
    {
        if (!ens[i].estEnVie())
            continue;

        Vec2 ep = ens[i].getPos();
        Vec2 ev = ens[i].getVit();

        ep.x += ev.x * dt * 0.5f;

        if (collisionTuileHitbox(niv, hitboxObjet16(ep)))
        {
            ev.x = -ev.x;
            ep.x += ev.x * dt * 0.5f;
        }

        ens[i].setPos(ep);
        ens[i].setVit(ev);

        Hitbox hbJoueur = hitboxJoueur(joueur.getPos());
        Hitbox hbEnnemi = hitboxObjet16(ens[i].getPos());

        if (hbJoueur.collision(hbEnnemi))
        {
            bool toucheParDessus =
                joueur.getPos().y < ens[i].getPos().y &&
                joueur.getVit().y > 0.f;

            if (toucheParDessus)
            {
                ens[i].eliminer();
                score += 100;

                Vec2 rebond = joueur.getVit();
                rebond.y = -180.f;
                joueur.setVit(rebond);
            }
            else
            {
                joueur.perdreVie();

                if (joueur.estMort())
                {
                    etat = EtatPartie::perdue;
                }
                else
                {
                    reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);

                    Vec2 spawn = sortirDuSol(niv, niv.getSpawn());

                    joueur.setPos(spawn);
                    joueur.setVit(Vec2(0.f, 0.f));
                }
            }
        }
    }

    if (in.tir)
    {
        boules.push_back(Feu(joueur.getPos(), joueur.getDir()));
    }

    for (unsigned int i = 0; i < boules.size(); i++)
    {
        boules[i].majTemps(dt);
    }

    for (unsigned int i = 0; i < items.size(); i++)
    {
        if (items[i].estDisponible())
        {
            Hitbox hbJoueur = hitboxJoueur(joueur.getPos());
            Hitbox hbItem = hitboxObjet16(items[i].getPos());

            if (hbJoueur.collision(hbItem))
            {
                items[i].prendre();
                score += 10;
            }
        }
    }

    temps -= dt;

    if (temps <= 0.f)
    {
        temps = 0.f;
        etat = EtatPartie::perdue;
    }

    Hitbox hbJoueur = hitboxJoueur(joueur.getPos());
    Hitbox hbFin = hitboxObjet16(niv.getFin());

    if (hbJoueur.collision(hbFin))
    {
        etat = EtatPartie::gagnee;
    }
}

float Jeu::getTemps() const
{
    return temps;
}

unsigned int Jeu::getScore() const
{
    return score;
}

EtatPartie Jeu::getEtat() const
{
    return etat;
}

const vector<Ennemi>& Jeu::getEnnemis() const
{
    return ens;
}

const vector<Item>& Jeu::getItems() const
{
    return items;
}

const vector<Feu>& Jeu::getBoules() const
{
    return boules;
}

void Jeu::testRegression()
{
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