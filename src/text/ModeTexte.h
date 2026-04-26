#ifndef MODETEXTE_H
#define MODETEXTE_H

#include "../core/Jeu.h"
#include "../core/Joueur.h"
#include "../core/Niveau.h"
#include "../core/EntreeJoueur.h"

/**
 * @brief Gère l'affichage et les commandes du jeu en mode console.
 *
 * Cette classe affiche une version simple du jeu dans le terminal.
 * Elle transforme aussi les commandes tapées par l'utilisateur en EntreeJoueur.
 */
class ModeTexte {
private:
    Jeu* jeu;        ///< Jeu utilisé par le mode texte.
    Niveau* niveau;  ///< Niveau affiché.
    Joueur* joueur;  ///< Joueur affiché.
    bool actif;      ///< Indique si la boucle continue.

public:
    /**
     * @brief Construit le mode texte.
     * @param j Jeu à utiliser.
     * @param n Niveau à afficher.
     * @param p Joueur à afficher.
     */
    ModeTexte(Jeu& j, Niveau& n, Joueur& p);

    /**
     * @brief Efface la console.
     */
    void effacerConsole() const;

    /**
     * @brief Affiche les informations principales.
     * @param meilleurScore Meilleur score du niveau.
     */
    void afficherInfos(const unsigned int meilleurScore) const;

    /**
     * @brief Affiche la carte du niveau.
     */
    void afficherCarte() const;

    /**
     * @brief Affiche les commandes disponibles.
     */
    void afficherCommandes() const;

    /**
     * @brief Affiche toute la frame.
     * @param meilleurScore Meilleur score du niveau.
     */
    void afficher(const unsigned int meilleurScore) const;

    /**
     * @brief Lit une commande utilisateur.
     * @param quitter Passe à true si le joueur veut quitter.
     * @return Entrée joueur correspondante.
     */
    EntreeJoueur lireEntree(bool& quitter) const;

    /**
     * @brief Lance la boucle du jeu en mode texte.
     * @param meilleurScore Meilleur score actuel du niveau.
     */
    void boucle(const unsigned int meilleurScore);
};

#endif
