#ifndef JOUEUR_H
#define JOUEUR_H

#include "Vec2.h"
#include "Entity.h"

/**
 * @brief Represente la taille ou le pouvoir du joueur.
 */
enum class TypeJoueur
{
    petit,
    grand,
    etoile,
    feu
};

/**
 * @brief Represente le personnage choisi.
 */
enum class Personnage
{
    mario,
    luigi
};

/**
 * @brief Représente l'état de vie du joueur.
 */
enum class EtatJoueur
{
    EnVie,
    Mort
};

/**
 * @brief Représente l'état visuel du mouvement du joueur.
 */
enum class EtatMouvJoueur
{
    rien,
    etat1,
    etat2,
    etat3
};

/**
 * @brief Représente le joueur.
 */
class Joueur : public Entity
{
private:
    TypeJoueur type;
    EtatJoueur etat;
    EtatMouvJoueur mouv;
    unsigned int vies;
    Personnage perso;

    float tempsEtoile;
    float tempsProtection;

public:
    Joueur() = delete;

    Joueur(const Vec2& p);

    ~Joueur() = default;

    void MajMouv();

    TypeJoueur getType() const;
    EtatJoueur getEtat() const;
    EtatMouvJoueur getMouv() const;
    unsigned int getVies() const;
    Personnage getPersonnage() const;
    float getTempsEtoile() const;

    void setPersonnage(Personnage p);
    void setType(TypeJoueur t);

    void devenirPetit();
    void devenirGrand();
    void devenirFeu();
    void devenirEtoile(float duree);

    void majPouvoirs(float dt);

    bool estInvincible() const;
    bool estProtege() const;
    bool peutTirerFeu() const;

    void commencerProtection(float duree);

    void perdreVie();
    void perdreVieDirecte();

    bool estMort() const;

    static void testRegression();
};

#endif