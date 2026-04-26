#ifndef JOUEUR_H
#define JOUEUR_H

#include "Vec2.h"
#include "Entity.h"

/**
 * @brief Represente la taille ou le pouvoir du joueur.
 */
enum class TypeJoueur {
    petit,  ///< Joueur normal.
    grand,  ///< Joueur agrandi.
    etoile, ///< Joueur invincible.
    feu     ///< Joueur avec pouvoir de feu.
};

/**
 * @brief Represente le personnage choisi.
 */
enum class Personnage {
    mario, ///< Skin Mario.
    luigi  ///< Skin Luigi.
};

/**
 * @brief Représente l'état de vie du joueur.
 */
enum class EtatJoueur {
    EnVie, ///< Joueur vivant.
    Mort   ///< Joueur mort.
};

/**
 * @brief Représente l'état visuel du mouvement du joueur.
 */
enum class EtatMouvJoueur {
    rien,  ///< Pas d'animation.
    etat1, ///< Premier état.
    etat2, ///< Deuxième état.
    etat3  ///< Troisième état.
};

/**
 * @brief Représente le joueur.
 */
class Joueur : public Entity {
private:
    TypeJoueur type; ///< Type actuel du joueur.
    EtatJoueur etat; ///< État de vie.
    EtatMouvJoueur mouv; ///< État de mouvement.
    unsigned int vies; ///< Nombre de vies restantes.
    Personnage perso; ///< Personnage choisi.

public:
    /**
     * @brief Empêche la création d'un joueur sans position.
     */
    Joueur() = delete;

    /**
     * @brief Construit un joueur à partir d'une position.
     * @param p Position initiale.
     */
    Joueur(const Vec2& p);

    /**
     * @brief Destructeur par défaut.
     */
    ~Joueur() = default;

    /**
     * @brief Met à jour l'état de mouvement du joueur.
     */
    void MajMouv();

    /**
     * @brief Retourne le type du joueur.
     * @return Type du joueur.
     */
    TypeJoueur getType() const;

    /**
     * @brief Retourne l'état du joueur.
     * @return État du joueur.
     */
    EtatJoueur getEtat() const;

    /**
     * @brief Retourne l'état de mouvement.
     * @return État de mouvement.
     */
    EtatMouvJoueur getMouv() const;

    /**
     * @brief Retourne le nombre de vies.
     * @return Nombre de vies.
     */
    unsigned int getVies() const;


    /**
    * @brief Retire une vie au joueur.
    */
    void perdreVie();

    /**
     * @brief Indique si le joueur n'a plus de vies.
     * @return true si le joueur est mort, false sinon.
     */
    bool estMort() const;

    /**
     * @brief Lance les tests de régression de Joueur.
     */
    static void testRegression();
};

#endif
