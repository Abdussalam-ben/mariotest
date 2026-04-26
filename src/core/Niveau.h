#ifndef NIVEAU_H
#define NIVEAU_H

#include <vector>
#include "Vec2.h"
#include "Ennemi.h"
#include "Item.h"
#include "PlateformeMobile.h"
#include "Tuile.h"

/**
 * @brief Représente les types de tuiles du niveau.
 */
enum class TypeTuile {
    vide,
    sol,
    cassable,
    incassable,
    tuyau,
    mystere
};

/**
 * @brief Représente un niveau du jeu.
 */
class Niveau {
private:
    unsigned int id; ///< Identifiant du niveau.
    unsigned int dimx; ///< Nombre de colonnes.
    unsigned int dimy; ///< Nombre de lignes.
    std::vector<TypeTuile> tuiles; ///< Carte du niveau.
    std::vector<TypeTuile> tuilesInit; ///< Carte initiale.
    Vec2 spawn; ///< Position de départ du joueur.
    Vec2 fin; ///< Position d'arrivée.
    std::vector<Ennemi> ensInit; ///< Ennemis initiaux.
    std::vector<Item> itemsInit; ///< Items initiaux.
    std::vector<PlateformeMobile> platsInit; ///< Plateformes initiales.
    std::vector<Tuile> mystInit; ///< Tuiles mystère initiales.

public:
    /**
     * @brief Construit un niveau vide.
     */
    Niveau();

    /**
     * @brief Construit un niveau simple.
     * @param i Identifiant.
     * @param nx Largeur en tuiles.
     * @param ny Hauteur en tuiles.
     */
    Niveau(const unsigned int i, const unsigned int nx, const unsigned int ny);

    /**
     * @brief Retourne l'identifiant.
     * @return Identifiant du niveau.
     */
    unsigned int getId() const;

    /**
     * @brief Retourne la largeur.
     * @return Nombre de colonnes.
     */
    unsigned int getDimX() const;

    /**
     * @brief Retourne la hauteur.
     * @return Nombre de lignes.
     */
    unsigned int getDimY() const;

    /**
     * @brief Retourne la position de départ.
     * @return Position de départ.
     */
    Vec2 getSpawn() const;

    /**
     * @brief Retourne la position de fin.
     * @return Position de fin.
     */
    Vec2 getFin() const;

    /**
     * @brief Modifie la position de départ.
     * @param p Nouvelle position.
     */
    void setSpawn(const Vec2& p);

    /**
     * @brief Modifie la position de fin.
     * @param p Nouvelle position.
     */
    void setFin(const Vec2& p);

    /**
     * @brief Retourne le type d'une tuile.
     * @param x Colonne.
     * @param y Ligne.
     * @return Type de la tuile.
     */
    TypeTuile getTuile(const unsigned int x, const unsigned int y) const;

    /**
     * @brief Modifie une tuile.
     * @param x Colonne.
     * @param y Ligne.
     * @param t Nouveau type.
     */
    void setTuile(const unsigned int x, const unsigned int y, const TypeTuile& t);

    /**
     * @brief Réinitialise la carte.
     */
    void reinit();

    /**
     * @brief Retourne les ennemis initiaux.
     * @return Référence constante vers les ennemis.
     */
    const std::vector<Ennemi>& getEnnemisInit() const;

    /**
     * @brief Retourne les items initiaux.
     * @return Référence constante vers les items.
     */
    const std::vector<Item>& getItemsInit() const;

    /**
     * @brief Retourne les plateformes initiales.
     * @return Référence constante vers les plateformes.
     */
    const std::vector<PlateformeMobile>& getPlatsInit() const;

    /**
     * @brief Retourne les tuiles mystère initiales.
     * @return Référence constante vers les tuiles.
     */
    const std::vector<Tuile>& getMystInit() const;

    /**
     * @brief Ajoute un ennemi initial.
     * @param e Ennemi à ajouter.
     */
    void ajouteEnnemi(const Ennemi& e);

    /**
     * @brief Ajoute un item initial.
     * @param i Item à ajouter.
     */
    void ajouteItem(const Item& i);

    /**
     * @brief Ajoute une plateforme initiale.
     * @param p Plateforme à ajouter.
     */
    void ajoutePlateforme(const PlateformeMobile& p);

    /**
     * @brief Ajoute une tuile mystère initiale.
     * @param t Tuile à ajouter.
     */
    void ajouteMystery(const Tuile& t);

    /**
     * @brief Lance les tests de régression.
     */
    static void testRegression();
};

#endif
