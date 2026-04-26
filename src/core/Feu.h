#ifndef FEU_H
#define FEU_H

#include "Entity.h"
#include "Constantes.h"

/**
 * @brief Represente l'etat d'une boule de feu.
 */
enum class EtatBouleFeu {
    active,  ///< Projectile actif.
    eteinte  ///< Projectile terminé.
};

/**
 * @brief Represente une boule de feu.
 */
class Feu : public Entity {
private:
    float temps; ///< Temps de vie restant.
    EtatBouleFeu etat; ///< État de la boule de feu.

public:
    /**
     * @brief Empêche la création sans paramètres.
     */
    Feu() = delete;

    /**
     * @brief Construit une boule de feu.
     * @param p Position initiale.
     * @param d Direction de départ.
     */
    Feu(const Vec2& p, const Direction& d);

    /**
     * @brief Retourne le temps de vie restant.
     * @return Temps restant.
     */
    float getTemps() const;

    /**
     * @brief Retourne l'état.
     * @return État de la boule de feu.
     */
    EtatBouleFeu getEtat() const;

    /**
     * @brief Indique si la boule de feu est active.
     * @return true si elle est encore active.
     */
    bool estActive() const;

    /**
     * @brief Met à jour le temps de vie.
     * @param dt Pas de temps.
     */
    void majTemps(const float dt);

    /**
     * @brief Éteint la boule de feu.
     */
    void eteindre();

    /**
     * @brief Lance les tests de régression.
     */
    static void testRegression();
};

#endif
