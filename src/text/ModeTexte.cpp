#include "ModeTexte.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

ModeTexte::ModeTexte(Jeu& j, Niveau& n, Joueur& p)
    : jeu(&j), niveau(&n), joueur(&p), actif(true) {}

void ModeTexte::effacerConsole() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ModeTexte::afficherInfos(const unsigned int meilleurScore) const {
    cout << "====================================" << endl;
    cout << "          SUPER MARIO TEXTE          " << endl;
    cout << "====================================" << endl;
    cout << "Niveau        : " << niveau->getId() << endl;
    cout << "Score actuel  : " << jeu->getScore() << endl;
    cout << "Meilleur score: " << meilleurScore << endl;
    cout << "Temps restant : " << jeu->getTemps() << endl;
    cout << "Vies          : " << joueur->getVies() << endl;
    cout << "Position      : (" << joueur->getPos().x << ", " << joueur->getPos().y << ")" << endl;
    cout << endl;
}

void ModeTexte::afficherCarte() const {
    const unsigned int largeur = niveau->getDimX();
    const unsigned int hauteur = niveau->getDimY();

    int px = static_cast<int>(joueur->getPos().x / 16.f);
    int py = static_cast<int>(joueur->getPos().y / 16.f);

    int fx = static_cast<int>(niveau->getFin().x / 16.f);
    int fy = static_cast<int>(niveau->getFin().y / 16.f);

    const vector<Ennemi>& ennemis = jeu->getEnnemis();
    const vector<Item>& items = jeu->getItems();

    for (unsigned int y = 0; y < hauteur; y++) {
        for (unsigned int x = 0; x < largeur; x++) {
            char symbole = '.';

            if (static_cast<int>(x) == px && static_cast<int>(y) == py) {
                symbole = 'J';
            }
            else if (static_cast<int>(x) == fx && static_cast<int>(y) == fy) {
                symbole = 'F';
            }
            else {
                bool trouve = false;

                for (unsigned int i = 0; i < ennemis.size(); i++) {
                    int ex = static_cast<int>(ennemis[i].getPos().x / 16.f);
                    int ey = static_cast<int>(ennemis[i].getPos().y / 16.f);

                    if (static_cast<int>(x) == ex && static_cast<int>(y) == ey) {
                        symbole = 'E';
                        trouve = true;
                    }
                }

                for (unsigned int i = 0; i < items.size() && !trouve; i++) {
                    int ix = static_cast<int>(items[i].getPos().x / 16.f);
                    int iy = static_cast<int>(items[i].getPos().y / 16.f);

                    if (static_cast<int>(x) == ix &&
                        static_cast<int>(y) == iy &&
                        items[i].estDisponible()) {

                        if (items[i].getType() == TypeItem::piece)
                            symbole = 'C';
                        else if (items[i].getType() == TypeItem::champignon)
                            symbole = 'M';
                        else if (items[i].getType() == TypeItem::fleur)
                            symbole = 'L';

                        trouve = true;
                    }
                }

                if (!trouve) {
                    TypeTuile t = niveau->getTuile(x, y);

                    if (t == TypeTuile::sol)
                        symbole = '#';
                    else if (t == TypeTuile::incassable)
                        symbole = '#';
                    else if (t == TypeTuile::cassable)
                        symbole = 'B';
                    else if (t == TypeTuile::tuyau)
                        symbole = 'T';
                    else if (t == TypeTuile::mystere)
                        symbole = '?';
                    else
                        symbole = '.';
                }
            }

            cout << symbole;
        }

        cout << endl;
    }
}

void ModeTexte::afficherCommandes() const {
    cout << endl;
    cout << "Commandes :" << endl;
    cout << "q  : gauche" << endl;
    cout << "d  : droite" << endl;
    cout << "z  : saut" << endl;
    cout << "f  : tirer" << endl;
    cout << "qz : gauche + saut" << endl;
    cout << "dz : droite + saut" << endl;
    cout << "n  : rien" << endl;
    cout << "e  : quitter" << endl;
    cout << endl;
}

void ModeTexte::afficher(const unsigned int meilleurScore) const {
    effacerConsole();
    afficherInfos(meilleurScore);
    afficherCarte();
    afficherCommandes();
}

EntreeJoueur ModeTexte::lireEntree(bool& quitter) const {
    EntreeJoueur entree;
    string cmd;

    cout << "Commande > ";
    cin >> cmd;

    if (cmd == "e") {
        quitter = true;
    }
    else if (cmd == "q") {
        entree.gauche = true;
    }
    else if (cmd == "d") {
        entree.droite = true;
    }
    else if (cmd == "z") {
        entree.saut = true;
    }
    else if (cmd == "f") {
        entree.tir = true;
    }
    else if (cmd == "qz" || cmd == "zq") {
        entree.gauche = true;
        entree.saut = true;
    }
    else if (cmd == "dz" || cmd == "zd") {
        entree.droite = true;
        entree.saut = true;
    }

    return entree;
}

void ModeTexte::boucle(const unsigned int meilleurScore) {
    actif = true;

    while (actif) {
        afficher(meilleurScore);

        bool quitter = false;
        EntreeJoueur entree = lireEntree(quitter);

        if (quitter) {
            actif = false;
        }
        else {
            jeu->maj(entree, 1.f / 10.f);
        }
    }

    cout << "Fin du mode texte." << endl;
}
