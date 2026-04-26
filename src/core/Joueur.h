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

public:
    Joueur() = delete;

    /**
     * @brief Construit un joueur à partir d'une position.
     * @param p Position initiale.
     */
    Joueur(const Vec2& p);

    ~Joueur() = default;

    /**
     * @brief Met à jour l'état de mouvement du joueur.
     */
    void MajMouv();

    TypeJoueur getType() const;
    EtatJoueur getEtat() const;
    EtatMouvJoueur getMouv() const;
    unsigned int getVies() const;

    /**
     * @brief Retourne le personnage choisi.
     * @return Mario ou Luigi.
     */
    Personnage getPersonnage() const;

    /**
     * @brief Change le personnage affiché.
     * @param p Nouveau personnage.
     */
    void setPersonnage(Personnage p);

    /**
     * @brief Retire une vie au joueur.
     */
    void perdreVie();

    /**
     * @brief Indique si le joueur n'a plus de vies.
     * @return true si le joueur est mort, false sinon.
     */
    bool estMort() const;

    static void testRegression();
};

#endif