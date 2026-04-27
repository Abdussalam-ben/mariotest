#include "Jeu.h"
#include "Hitbox.h"
#include "Constantes.h"

#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

static const float TAILLE_TUILE = 16.f;
static const float VITESSE_SAUT_JEU = 340.f;
static const float DUREE_ETOILE = 5.f;
static const unsigned int MAX_BOULES_FEU = 5;
static const float GRAVITE_FEU = 650.f;
static const float REBOND_FEU_Y = 180.f;

/*
 * Hitbox plus fine que le sprite pour que le joueur passe mieux
 * dans les espaces serrés.
 */
static const float HITBOX_JOUEUR_X = 3.f;
static const float HITBOX_JOUEUR_Y = 0.f;
static const float HITBOX_JOUEUR_L = 10.f;

/**
 * @brief Indique si une tuile bloque les déplacements.
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
 * @brief Indique si une case de la carte est solide.
 */
static bool tuileSolide(const Niveau& n, int x, int y)
{
    /*
     * Gauche / droite / haut hors carte = mur solide.
     * Bas hors carte = vide, pour permettre la chute.
     */
    if (x < 0 || y < 0)
        return true;

    if (x >= static_cast<int>(n.getDimX()))
        return true;

    if (y >= static_cast<int>(n.getDimY()))
        return false;

    return estSolide(n.getTuile(static_cast<unsigned int>(x), static_cast<unsigned int>(y)));
}

/**
 * @brief Initialise l'aléatoire une seule fois.
 */
static void initRandom()
{
    static bool initialise = false;

    if (!initialise)
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        initialise = true;
    }
}

/**
 * @brief Indique si le joueur peut casser un bloc cassable.
 */
static bool joueurPeutCasserBloc(const Joueur& joueur)
{
    return joueur.getType() == TypeJoueur::grand ||
           joueur.getType() == TypeJoueur::feu;
}

/**
 * @brief Fait apparaître un item ou rarement un ennemi depuis une tuile mystère.
 */
static void faireApparaitreMystere(vector<Item>& items,
                                   vector<Ennemi>& ens,
                                   int tx,
                                   int ty)
{
    initRandom();

    Vec2 position(
        static_cast<float>(tx) * TAILLE_TUILE,
        static_cast<float>(ty - 1) * TAILLE_TUILE
    );

    if (ty <= 0)
        position.y = static_cast<float>(ty) * TAILLE_TUILE;

    int chance = rand() % 100;

    /*
     * 15% de chance de faire apparaître un ennemi.
     */
    if (chance < 15)
    {
        ens.push_back(Ennemi(position, TypeEnnemi::Goomba));
        return;
    }

    /*
     * Sinon, apparition d'un item aléatoire.
     */
    int item = rand() % 4;

    if (item == 0)
        items.push_back(Item(position, TypeItem::piece));
    else if (item == 1)
        items.push_back(Item(position, TypeItem::champignon));
    else if (item == 2)
        items.push_back(Item(position, TypeItem::fleur));
    else
        items.push_back(Item(position, TypeItem::etoile));
}

/**
 * @brief Gère une tuile frappée par le joueur par-dessous.
 */
static void gererTuileFrappeeParDessous(Niveau& niv,
                                        Joueur& joueur,
                                        vector<Item>& items,
                                        vector<Ennemi>& ens,
                                        int tx,
                                        int ty)
{
    if (tx < 0 || ty < 0)
        return;

    if (tx >= static_cast<int>(niv.getDimX()) ||
        ty >= static_cast<int>(niv.getDimY()))
        return;

    TypeTuile t = niv.getTuile(static_cast<unsigned int>(tx),
                               static_cast<unsigned int>(ty));

    /*
     * Bloc cassable :
     * seulement Grand ou Feu peut le casser.
     */
    if (t == TypeTuile::cassable)
    {
        if (joueurPeutCasserBloc(joueur))
        {
            niv.setTuile(static_cast<unsigned int>(tx),
                         static_cast<unsigned int>(ty),
                         TypeTuile::vide);
        }

        return;
    }

    /*
     * Tuile mystère :
     * elle disparaît et fait apparaître un item ou rarement un ennemi.
     */
    if (t == TypeTuile::mystere)
    {
        niv.setTuile(static_cast<unsigned int>(tx),
                     static_cast<unsigned int>(ty),
                     TypeTuile::vide);

        faireApparaitreMystere(items, ens, tx, ty);
    }
}

/**
 * @brief Retourne la hitbox du joueur.
 */
static Hitbox hitboxJoueur(const Joueur& joueur, const Vec2& p)
{
    return Hitbox(
        p.x + HITBOX_JOUEUR_X,
        p.y + HITBOX_JOUEUR_Y,
        HITBOX_JOUEUR_L,
        joueur.getHauteur()
    );
}

/**
 * @brief Retourne une hitbox 16x16.
 */
static Hitbox hitboxObjet16(const Vec2& p)
{
    return Hitbox(p.x, p.y, 16.f, 16.f);
}

/**
 * @brief Retourne la hitbox complète du drapeau de fin.
 */
static Hitbox hitboxDrapeauFin(const Vec2& p)
{
    /*
     * Le sprite du drapeau est affiché en 16x176 dans GameGraphicMode.cpp :
     * afficherSprite("drapeau_fin.png", fx, fy - 160, 16, 176);
     * La collision doit donc utiliser la même position et la même taille.
     */
    return Hitbox(p.x, p.y - 160.f, 16.f, 176.f);
}

/**
 * @brief Retourne la hitbox d'une boule de feu.
 */
static Hitbox hitboxFeu(const Vec2& p)
{
    return Hitbox(p.x, p.y, 8.f, 8.f);
}

/**
 * @brief Retourne la hitbox d'un ennemi.
 */
static Hitbox hitboxEnnemi(const Ennemi& e, const Vec2& p)
{
    if (e.getType() == TypeEnnemi::Koopa)
        return Hitbox(p.x, p.y, 16.f, 24.f);

    return Hitbox(p.x, p.y, 16.f, 16.f);
}

/**
 * @brief Indique si une hitbox touche une tuile solide.
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
 * @brief Sort le joueur d'un sol si sa position est légèrement bloquée.
 */
static Vec2 sortirDuSol(const Niveau& n, const Joueur& joueur, Vec2 p)
{
    Hitbox h = hitboxJoueur(joueur, p);

    while (collisionTuileHitbox(n, h) && p.y > 0.f)
    {
        p.y -= 1.f;
        h = hitboxJoueur(joueur, p);
    }

    return p;
}

/**
 * @brief Corrige la collision horizontale du joueur.
 */
static void corrigerCollisionHorizontale(const Niveau& n,
                                         const Joueur& joueur,
                                         Vec2& p,
                                         Vec2& v)
{
    Hitbox h = hitboxJoueur(joueur, p);

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
                p.x = static_cast<float>(droite) * TAILLE_TUILE -
                      HITBOX_JOUEUR_L - HITBOX_JOUEUR_X;
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
                p.x = static_cast<float>(gauche + 1) * TAILLE_TUILE -
                      HITBOX_JOUEUR_X;
                v.x = 0.f;
                return;
            }
        }
    }
}

/**
 * @brief Corrige la collision verticale du joueur.
 */
static void corrigerCollisionVerticale(Niveau& n,
                                       Joueur& joueur,
                                       vector<Item>& items,
                                       vector<Ennemi>& ens,
                                       Vec2& p,
                                       Vec2& v)
{
    Hitbox h = hitboxJoueur(joueur, p);

    if (!collisionTuileHitbox(n, h))
        return;

    int gauche = static_cast<int>(h.x / TAILLE_TUILE);
    int droite = static_cast<int>((h.x + h.largeur - 1.f) / TAILLE_TUILE);

    /*
     * Collision en tombant : on pose le joueur sur le sol.
     */
    if (v.y > 0.f)
    {
        int bas = static_cast<int>((h.y + h.hauteur - 1.f) / TAILLE_TUILE);

        for (int x = gauche; x <= droite; x++)
        {
            if (tuileSolide(n, x, bas))
            {
                p.y = static_cast<float>(bas) * TAILLE_TUILE -
                      h.hauteur - HITBOX_JOUEUR_Y;
                v.y = 0.f;
                return;
            }
        }
    }

    /*
     * Collision en sautant : le joueur tape une tuile par dessous.
     */
    else if (v.y < 0.f)
    {
        int haut = static_cast<int>(h.y / TAILLE_TUILE);

        for (int x = gauche; x <= droite; x++)
        {
            if (tuileSolide(n, x, haut))
            {
                gererTuileFrappeeParDessous(n, joueur, items, ens, x, haut);

                p.y = static_cast<float>(haut + 1) * TAILLE_TUILE -
                      HITBOX_JOUEUR_Y;
                v.y = 0.f;
                return;
            }
        }
    }
}

/**
 * @brief Corrige la collision horizontale d'un ennemi.
 */
static void corrigerCollisionEnnemiHorizontale(const Niveau& n,
                                               const Ennemi& e,
                                               Vec2& p,
                                               Vec2& v)
{
    Hitbox h = hitboxEnnemi(e, p);

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
                p.x = static_cast<float>(droite) * TAILLE_TUILE - h.largeur;
                v.x = -v.x;
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
                p.x = static_cast<float>(gauche + 1) * TAILLE_TUILE;
                v.x = -v.x;
                return;
            }
        }
    }
}

/**
 * @brief Corrige la collision verticale d'un ennemi.
 */
static void corrigerCollisionEnnemiVerticale(const Niveau& n,
                                             const Ennemi& e,
                                             Vec2& p,
                                             Vec2& v)
{
    Hitbox h = hitboxEnnemi(e, p);

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
 * @brief Indique si le joueur est au sol.
 */
static bool joueurAuSol(const Niveau& n, const Joueur& joueur, const Vec2& p)
{
    Hitbox h = hitboxJoueur(joueur, p);
    h.y += 1.f;

    return collisionTuileHitbox(n, h);
}

/**
 * @brief Compte les boules de feu actives.
 */
static unsigned int nombreBoulesActives(const vector<Feu>& boules)
{
    unsigned int n = 0;

    for (unsigned int i = 0; i < boules.size(); i++)
    {
        if (boules[i].estActive())
            n++;
    }

    return n;
}

/**
 * @brief Supprime les boules de feu inactives.
 */
static void nettoyerBoules(vector<Feu>& boules)
{
    vector<Feu> boulesActives;

    for (unsigned int i = 0; i < boules.size(); i++)
    {
        if (boules[i].estActive())
            boulesActives.push_back(boules[i]);
    }

    boules = boulesActives;
}

/**
 * @brief Réinitialise les éléments du niveau.
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

/**
 * @brief Construit une partie.
 */
Jeu::Jeu(Niveau& n, Joueur& j)
    : niv(n),
      joueur(j),
      temps(300.f),
      score(0),
      pieces(0),
      etat(EtatPartie::enCours)
{
    reinit();
}

/**
 * @brief Réinitialise la partie.
 */
void Jeu::reinit()
{
    reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);

    score = 0;
    pieces = 0;
    etat = EtatPartie::enCours;

    joueur.devenirPetit();

    Vec2 spawn = sortirDuSol(niv, joueur, niv.getSpawn());

    joueur.setPos(spawn);
    joueur.setVit(Vec2(0.f, 0.f));
}

/**
 * @brief Met à jour la partie.
 */
void Jeu::maj(const EntreeJoueur& in, const float dt)
{
    assert(dt >= 0.f);

    if (etat != EtatPartie::enCours)
        return;

    joueur.majPouvoirs(dt);

    Vec2 v = joueur.getVit();
    Vec2 p = joueur.getPos();

    p = sortirDuSol(niv, joueur, p);

    bool auSol = joueurAuSol(niv, joueur, p);

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

    corrigerCollisionHorizontale(niv, joueur, p, v);

    /*
     * Déplacement vertical.
     */
    p.y += v.y * dt;

    if (p.y < 0.f)
    {
        p.y = 0.f;
        v.y = 0.f;
    }

    corrigerCollisionVerticale(niv, joueur, items, ens, p, v);

    p = sortirDuSol(niv, joueur, p);

    /*
     * Chute hors de la map = perte directe d'une vie,
     * même avec étoile.
     */
    if (p.y > static_cast<float>(niv.getDimY()) * TAILLE_TUILE)
    {
        joueur.perdreVieDirecte();

        if (joueur.estMort())
        {
            etat = EtatPartie::perdue;
        }
        else
        {
            reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);

            joueur.devenirPetit();

            p = sortirDuSol(niv, joueur, niv.getSpawn());
            v = Vec2(0.f, 0.f);
        }
    }

    joueur.setVit(v);
    joueur.setPos(p);
    joueur.MajMouv();

    /*
     * Plateformes.
     */
    for (unsigned int i = 0; i < plats.size(); i++)
    {
        plats[i].maj(dt);
    }

    /*
     * Ennemis avec gravité.
     */
    for (unsigned int i = 0; i < ens.size(); i++)
    {
        if (!ens[i].estEnVie())
            continue;

        Vec2 ep = ens[i].getPos();
        Vec2 ev = ens[i].getVit();

        /*
         * Gravité des ennemis.
         */
        ev.y += GRAVITE * dt;

        /*
         * Déplacement horizontal.
         */
        ep.x += ev.x * dt * 0.5f;
        corrigerCollisionEnnemiHorizontale(niv, ens[i], ep, ev);

        /*
         * Déplacement vertical.
         */
        ep.y += ev.y * dt;
        corrigerCollisionEnnemiVerticale(niv, ens[i], ep, ev);

        /*
         * Si un ennemi tombe hors de la map, on l'élimine.
         */
        if (ep.y > static_cast<float>(niv.getDimY()) * TAILLE_TUILE)
        {
            ens[i].eliminer();
            continue;
        }

        ens[i].setPos(ep);
        ens[i].setVit(ev);

        Hitbox hbJoueur = hitboxJoueur(joueur, joueur.getPos());
        Hitbox hbEnnemi = hitboxEnnemi(ens[i], ens[i].getPos());

        if (hbJoueur.collision(hbEnnemi))
        {
            bool toucheParDessus =
                joueur.getPos().y + joueur.getHauteur() <= ens[i].getPos().y + 8.f &&
                joueur.getVit().y > 0.f;

            if (toucheParDessus || joueur.estInvincible())
            {
                ens[i].eliminer();
                score += 100;

                if (toucheParDessus)
                {
                    Vec2 rebond = joueur.getVit();
                    rebond.y = -180.f;
                    joueur.setVit(rebond);
                }
            }
            else
            {
                if (joueur.estProtege())
                    continue;

                unsigned int viesAvant = joueur.getVies();
                TypeJoueur typeAvant = joueur.getType();

                joueur.perdreVie();

                if (joueur.estMort())
                {
                    etat = EtatPartie::perdue;
                }
                else
                {
                    /*
                     * Grand/Feu touché : il devient petit, reste dans le niveau.
                     */
                    if (typeAvant == TypeJoueur::grand || typeAvant == TypeJoueur::feu)
                    {
                        Vec2 corrigee = sortirDuSol(niv, joueur, joueur.getPos());

                        joueur.setPos(corrigee);
                        joueur.setVit(Vec2(0.f, 0.f));
                    }

                    /*
                     * Petit touché : il perd une vie et le niveau reset.
                     */
                    else if (viesAvant > joueur.getVies())
                    {
                        reinitialiserObjets(niv, ens, items, plats, myst, boules, temps);

                        joueur.devenirPetit();

                        Vec2 spawn = sortirDuSol(niv, joueur, niv.getSpawn());

                        joueur.setPos(spawn);
                        joueur.setVit(Vec2(0.f, 0.f));
                    }
                }
            }
        }
    }

    /*
     * Tir de feu.
     * Maximum 5 boules actives à l'écran.
     * Il faut relâcher F avant de tirer une autre boule.
     */
    static bool tirDejaAppuye = false;

    if (!in.tir)
        tirDejaAppuye = false;

    if (in.tir &&
        !tirDejaAppuye &&
        joueur.peutTirerFeu() &&
        nombreBoulesActives(boules) < MAX_BOULES_FEU)
    {
        Vec2 depart = joueur.getPos();

        if (joueur.getDir() == Direction::Droite)
            depart.x += 14.f;
        else
            depart.x -= 6.f;

        depart.y += joueur.getHauteur() / 2.f;

        boules.push_back(Feu(depart, joueur.getDir()));
        tirDejaAppuye = true;
    }

    /*
     * Boules de feu.
     */
    for (unsigned int i = 0; i < boules.size(); i++)
    {
        if (!boules[i].estActive())
            continue;

        boules[i].majTemps(dt);

        Vec2 bp = boules[i].getPos();
        Vec2 bv = boules[i].getVit();

        /*
         * Gravité : la boule descend puis rebondit.
         */
        bv.y += GRAVITE_FEU * dt;

        /*
         * Collision horizontale : mur / tuyau / bloc.
         */
        Vec2 ancienneBp = bp;

        bp.x += bv.x * dt;
        boules[i].setPos(bp);

        if (collisionTuileHitbox(niv, hitboxFeu(bp)))
        {
            bp = ancienneBp;
            bv.x = -bv.x;
            boules[i].setPos(bp);
        }

        /*
         * Collision verticale : sol / plafond.
         */
        ancienneBp = bp;

        bp.y += bv.y * dt;
        boules[i].setPos(bp);

        if (collisionTuileHitbox(niv, hitboxFeu(bp)))
        {
            bp = ancienneBp;

            if (bv.y > 0.f)
                bv.y = -REBOND_FEU_Y;
            else
                bv.y = 90.f;

            boules[i].setPos(bp);
        }

        boules[i].setVit(bv);

        /*
         * Collision boule de feu / ennemi.
         */
        for (unsigned int j = 0; j < ens.size(); j++)
        {
            if (!ens[j].estEnVie())
                continue;

            Hitbox hbFeu = hitboxFeu(boules[i].getPos());
            Hitbox hbEnnemi = hitboxEnnemi(ens[j], ens[j].getPos());

            if (hbFeu.collision(hbEnnemi))
            {
                ens[j].eliminer();
                boules[i].eteindre();
                score += 100;
                break;
            }
        }
    }

    nettoyerBoules(boules);

    /*
     * Items.
     */
    for (unsigned int i = 0; i < items.size(); i++)
    {
        if (!items[i].estDisponible())
            continue;

        Hitbox hbJoueur = hitboxJoueur(joueur, joueur.getPos());
        Hitbox hbItem = hitboxObjet16(items[i].getPos());

        if (hbJoueur.collision(hbItem))
        {
            TypeItem typeItem = items[i].getType();

            items[i].prendre();

            if (typeItem == TypeItem::piece)
            {
                pieces++;
                score += 10;
            }
            else if (typeItem == TypeItem::champignon)
            {
                joueur.devenirGrand();
                score += 50;
            }
            else if (typeItem == TypeItem::fleur)
            {
                joueur.devenirFeu();
                score += 50;
            }
            else if (typeItem == TypeItem::etoile)
            {
                joueur.devenirEtoile(DUREE_ETOILE);
                score += 50;
            }

            Vec2 posCorrigee = sortirDuSol(niv, joueur, joueur.getPos());
            joueur.setPos(posCorrigee);
        }
    }

    /*
     * Temps.
     */
    temps -= dt;

    if (temps <= 0.f)
    {
        temps = 0.f;
        etat = EtatPartie::perdue;
    }

    /*
     * Arrivée.
     * Le drapeau a une hitbox 16x176 pour permettre de gagner
     * en touchant n'importe quelle partie du drapeau, même par-dessus.
     */
    Hitbox hbJoueur = hitboxJoueur(joueur, joueur.getPos());
    Hitbox hbFin = hitboxDrapeauFin(niv.getFin());

    if (hbJoueur.collision(hbFin))
    {
        etat = EtatPartie::gagnee;
    }
}

/**
 * @brief Retourne le temps restant.
 */
float Jeu::getTemps() const
{
    return temps;
}

/**
 * @brief Retourne le score.
 */
unsigned int Jeu::getScore() const
{
    return score;
}

/**
 * @brief Retourne le nombre de pièces ramassées.
 */
unsigned int Jeu::getPieces() const
{
    return pieces;
}


/**
 * @brief Retourne l'état de la partie.
 */
EtatPartie Jeu::getEtat() const
{
    return etat;
}

/**
 * @brief Retourne les ennemis actifs.
 */
const vector<Ennemi>& Jeu::getEnnemis() const
{
    return ens;
}

/**
 * @brief Retourne les items actifs.
 */
const vector<Item>& Jeu::getItems() const
{
    return items;
}

/**
 * @brief Retourne les boules de feu actives.
 */
const vector<Feu>& Jeu::getBoules() const
{
    return boules;
}

/**
 * @brief Test de régression.
 */
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

    cout << "Tous les tests de Jeu sont valides." << endl;
}