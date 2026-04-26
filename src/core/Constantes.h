#ifndef CONSTANTES_H
#define CONSTANTES_H

#include <string>
#include <vector>

/**
 * @file Constantes.h
 * @brief Regroupe les constantes globales du jeu.
 */

/** @brief Nombre de vies initiales du joueur. */
const unsigned int NB_VIES_JOUEUR = 3;

/** @brief Largeur du joueur en pixels. */
const float LARGEUR_JOUEUR = 16.f;

/** @brief Hauteur du joueur en pixels. */
const float HAUTEUR_JOUEUR = 16.f;

/** @brief Hauteur du joueur lorsqu'il est grand. */
const float HAUTEUR_JOUEUR_GRAND = 32.f;

/** @brief Vitesse horizontale maximale du joueur. */
const float V_MAX_JOUEUR_X = 150.f;

/** @brief Accélération horizontale du joueur. */
const float ACC_JOUEUR_X = 2400.f;

/** @brief Gravité appliquée au joueur. */
const float GRAVITE = 1200.f;

/** @brief Vitesse initiale du saut. */
const float V_SAUT = 420.f;

/** @brief Temps entre deux changements d'état visuel. */
const unsigned int TEMPS_MAJ_MOUV = 120;

/** @brief Largeur d'un ennemi. */
const float LARGEUR_ENNEMI = 16.f;

/** @brief Hauteur d'un ennemi. */
const float HAUTEUR_ENNEMI = 16.f;

/** @brief Vitesse horizontale de base d'un ennemi. */
const float V_ENNEMI_X = 80.f;


/**
 * @brief Largeur réelle de la fenêtre SDL affichée à l'écran.
 */
const unsigned int WINDOW_WIDTH = 768;

/**
 * @brief Hauteur réelle de la fenêtre SDL affichée à l'écran.
 */
const unsigned int WINDOW_HEIGHT = 720;

/**
 * @brief Largeur logique du rendu de style NES.
 */
const unsigned int WINDOW_NES_WIDTH = 256;

/**
 * @brief Hauteur logique du rendu de style NES.
 */
const unsigned int WINDOW_NES_HEIGHT = 240;

#endif
