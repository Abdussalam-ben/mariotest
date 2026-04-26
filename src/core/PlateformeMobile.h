#ifndef PLATFORMEMOBILE_H
#define PLATFORMEMOBILE_H

#include "Entity.h"

/**
 * @brief Représente l'axe de déplacement d'une plateforme mobile.
 */
enum class AxePlateforme {
    horizontal, ///< Deplacement sur l'axe x.
    vertical    ///< Deplacement sur l'axe y.
};

/**
 * @brief Représente une plateforme mobile.
 */
class PlateformeMobile : public Entity {
private:
    Vec2 dep; ///< Position de départ.
    float amp; ///< Amplitude du mouvement.
    AxePlateforme axe; ///< Axe du mouvement.
    bool aller; ///< Sens actuel.

public:
    /**
     * @brief Empêche la création sans paramètres.
     */
    PlateformeMobile() = delete;

    /**
     * @brief Construit une plateforme mobile.
     * @param p Position de départ.
     * @param a Amplitude.
     * @param ax Axe du mouvement.
     */
    PlateformeMobile(const Vec2& p, const float a, const AxePlateforme& ax);

    /**
     * @brief Met à jour la position de la plateforme.
     * @param dt Pas de temps.
     */
    void maj(const float dt);

    /**
     * @brief Retourne la position de départ.
     * @return Position de départ.
     */
    Vec2 getDep() const;

    /**
     * @brief Retourne l'amplitude.
     * @return Amplitude du mouvement.
     */
    float getAmp() const;

    /**
     * @brief Retourne l'axe.
     * @return Axe du mouvement.
     */
    AxePlateforme getAxe() const;

    /**
     * @brief Lance les tests de régression.
     */
    static void testRegression();
};

#endif
