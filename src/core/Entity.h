#ifndef ENTITY_H
#define ENTITY_H

#include "Vec2.h"

/**
 * @brief Représente la direction horizontale d'une entité.
 */
enum class Direction {
    Droite, ///< Direction vers la droite.
    Gauche  ///< Direction vers la gauche.
};

/**
 * @brief Classe de base pour les objets mobiles du jeu.
 */
class Entity {
protected:
    Vec2 pos; ///< Position de l'entité.
    Vec2 v;   ///< Vitesse de l'entité.
    float l;  ///< Largeur de l'entité.
    float h;  ///< Hauteur de l'entité.
    Direction dir; ///< Direction de l'entité.

public:
    /**
     * @brief Construit une entité par défaut.
     */
    Entity();

    /**
     * @brief Destructeur de la classe Entity.
     */
    virtual ~Entity();

    /**
     * @brief Retourne la position.
     * @return Position de l'entité.
     */
    Vec2 getPos() const;

    /**
     * @brief Retourne la vitesse.
     * @return Vitesse de l'entité.
     */
    Vec2 getVit() const;

    /**
     * @brief Retourne la direction.
     * @return Direction actuelle.
     */
    Direction getDir() const;

    /**
     * @brief Retourne la largeur.
     * @return Largeur de l'entité.
     */
    float getLargeur() const;

    /**
     * @brief Retourne la hauteur.
     * @return Hauteur de l'entité.
     */
    float getHauteur() const;

    /**
     * @brief Ajoute une variation à la vitesse actuelle.
     * @param dv Variation de vitesse.
     */
    void updateVit(const Vec2& dv);

    /**
     * @brief Modifie la position.
     * @param p Nouvelle position.
     */
    void setPos(const Vec2& p);

    /**
     * @brief Modifie la vitesse.
     * @param nv Nouvelle vitesse.
     */
    void setVit(const Vec2& nv);

    /**
     * @brief Modifie la direction.
     * @param d Nouvelle direction.
     */
    void setDir(const Direction& d);

    /**
     * @brief Modifie la largeur.
     * @param nl Nouvelle largeur.
     */
    void setLargeur(const float nl);

    /**
     * @brief Modifie la hauteur.
     * @param nh Nouvelle hauteur.
     */
    void setHauteur(const float nh);

    /**
     * @brief Lance les tests de régression de Entity.
     */
    static void testRegression();
};

#endif
