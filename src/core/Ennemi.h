#ifndef ENNEMI_H
#define ENNEMI_H

#include "Entity.h"

/**
 * @brief Représente les types d'ennemis du jeu.
 */
enum class TypeEnnemi {
    Koopa,  ///< Ennemi de type Koopa.
    Goomba  ///< Ennemi de type Goomba.
};

/**
 * @brief Représente l'état de vie d'un ennemi.
 */
enum class EtatEnnemi {
    EnVie, ///< Ennemi vivant.
    Mort   ///< Ennemi éliminé.
};

/**
 * @brief Représente l'état visuel du mouvement d'un ennemi.
 */
enum class EtatMouvEnnemi {
    etat1, ///< Premier état.
    etat2  ///< Deuxième état.
};

/**
 * @brief Représente un ennemi du jeu.
 */
class Ennemi : public Entity {
private:
    TypeEnnemi type; ///< Type de l'ennemi.
    EtatEnnemi etat; ///< État de vie.
    EtatMouvEnnemi mouv; ///< État de mouvement.
    unsigned int dtMouv; ///< Temps écoulé depuis la dernière mise à jour du mouvement.

public:
    /**
     * @brief Empêche la création d'un ennemi sans paramètres.
     */
    Ennemi() = delete;

    /**
     * @brief Construit un ennemi.
     * @param p Position initiale.
     * @param t Type de l'ennemi.
     */
    Ennemi(const Vec2& p, const TypeEnnemi t);

    /**
     * @brief Destructeur par défaut.
     */
    ~Ennemi() = default;

    /**
     * @brief Retourne le type de l'ennemi.
     * @return Type de l'ennemi.
     */
    TypeEnnemi getType() const;

    /**
     * @brief Retourne l'état de mouvement.
     * @return État du mouvement.
     */
    EtatMouvEnnemi getMouv() const;

    /**
     * @brief Retourne l'état de vie.
     * @return État de l'ennemi.
     */
    EtatEnnemi getEtat() const;

    /**
     * @brief Indique si l'ennemi est vivant.
     * @return true si l'ennemi est vivant, false sinon.
     */
    bool estEnVie() const;

    /**
     * @brief Passe à l'état de mouvement suivant.
     */
    void mettreAJourMouvement();

    /**
     * @brief Élimine l'ennemi.
     */
    void eliminer();

    /**
     * @brief Retourne le temps depuis la dernière mise à jour du mouvement.
     * @return Temps écoulé.
     */
    unsigned int getDtDepuisMajMouvement() const;

    /**
     * @brief Modifie le temps depuis la dernière mise à jour du mouvement.
     * @param nDt Nouvelle valeur.
     */
    void mettreAJourDtDepuisMajMouvement(const unsigned int nDt);

    /**
     * @brief Lance les tests de régression de Ennemi.
     */
    static void testRegression();
};

#endif
