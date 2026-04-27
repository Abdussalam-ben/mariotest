#include "GameGraphicMode.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Construit le mode graphique du jeu.
 */
GameGraphicMode::GameGraphicMode(SDLContext& c, TextureManager& t)
    : contexte(&c),
      textures(&t),
      jeu(nullptr),
      niveau(nullptr),
      joueur(nullptr),
      actif(true),
      enPause(false),
      frame(0),
      cameraX(0.f),
      cameraY(0.f),
      police(nullptr),
      scoreMax(0)
{
    initialiserPolice();
}

/**
 * @brief Associe le jeu, le niveau et le joueur au mode graphique.
 */
void GameGraphicMode::setJeu(Jeu& j, Niveau& n, Joueur& p)
{
    jeu = &j;
    niveau = &n;
    joueur = &p;
    scoreMax = calculerScoreMax();
}

/**
 * @brief Lit les touches du clavier.
 */
EntreeJoueur GameGraphicMode::lireEntree() const
{
    EntreeJoueur entree;

    const Uint8* clavier = SDL_GetKeyboardState(NULL);

    entree.gauche = clavier[SDL_SCANCODE_Q] || clavier[SDL_SCANCODE_LEFT];
    entree.droite = clavier[SDL_SCANCODE_D] || clavier[SDL_SCANCODE_RIGHT];
    entree.saut = clavier[SDL_SCANCODE_SPACE] ||
                  clavier[SDL_SCANCODE_Z] ||
                  clavier[SDL_SCANCODE_UP];
    entree.tir = clavier[SDL_SCANCODE_F];

    return entree;
}

/**
 * @brief Met à jour la caméra selon la position du joueur.
 */
void GameGraphicMode::majCamera()
{
    if (joueur == nullptr || niveau == nullptr)
        return;

    const float largeurEcran = static_cast<float>(WINDOW_NES_WIDTH);
    const float hauteurEcran = static_cast<float>(WINDOW_NES_HEIGHT);

    const float largeurNiveau = static_cast<float>(niveau->getDimX() * 16);
    const float hauteurNiveau = static_cast<float>(niveau->getDimY() * 16);

    cameraX = joueur->getPos().x - largeurEcran / 2.f;
    cameraY = joueur->getPos().y - hauteurEcran / 2.f;

    if (cameraX < 0.f)
        cameraX = 0.f;

    if (cameraY < 0.f)
        cameraY = 0.f;

    if (cameraX > largeurNiveau - largeurEcran)
        cameraX = largeurNiveau - largeurEcran;

    if (cameraY > hauteurNiveau - hauteurEcran)
        cameraY = hauteurNiveau - hauteurEcran;

    if (cameraX < 0.f)
        cameraX = 0.f;

    if (cameraY < 0.f)
        cameraY = 0.f;
}

/**
 * @brief Initialise la police Mario utilisée pour le HUD et la pause.
 */
void GameGraphicMode::initialiserPolice()
{
    if (TTF_WasInit() == 0)
    {
        if (TTF_Init() == -1)
        {
            cerr << "Erreur TTF_Init : " << TTF_GetError() << endl;
            police = nullptr;
            return;
        }
    }

    const char* cheminPolice = "assets/polices/super_mario_bros_nes.ttf";

    police = TTF_OpenFont(cheminPolice, 8);

    if (police == nullptr)
    {
        cerr << "Erreur : impossible de charger la police " << cheminPolice << endl;
        cerr << TTF_GetError() << endl;
    }
}

/**
 * @brief Ferme la police utilisée.
 */
void GameGraphicMode::fermerPolice()
{
    if (police != nullptr)
    {
        TTF_CloseFont(police);
        police = nullptr;
    }
}

/**
 * @brief Affiche un sprite à l'écran.
 */
void GameGraphicMode::afficherSprite(const char* nom, int x, int y, int l, int h, bool flip)
{
    string chemin = string("assets/sprites/") + nom;

    SDL_Texture* tex = textures->getTexture(chemin);

    if (tex == nullptr)
        return;

    SDL_Rect dst;
    dst.x = x - static_cast<int>(cameraX);
    dst.y = y - static_cast<int>(cameraY);
    dst.w = l;
    dst.h = h;

    SDL_RendererFlip f = flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_RenderCopyEx(contexte->getRenderer(), tex, NULL, &dst, 0.0, NULL, f);
}

/**
 * @brief Affiche un texte à une position précise.
 */
void GameGraphicMode::afficherTexte(const string& texte, int x, int y)
{
    if (police == nullptr)
        return;

    SDL_Color couleur;
    couleur.r = 255;
    couleur.g = 255;
    couleur.b = 255;
    couleur.a = 255;

    SDL_Surface* surface = TTF_RenderText_Blended(police, texte.c_str(), couleur);

    if (surface == nullptr)
        return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(contexte->getRenderer(), surface);

    if (texture == nullptr)
    {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = surface->w;
    dst.h = surface->h;

    SDL_RenderCopy(contexte->getRenderer(), texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

/**
 * @brief Affiche un texte centré horizontalement.
 */
void GameGraphicMode::afficherTexteCentre(const string& texte, int y)
{
    if (police == nullptr)
        return;

    int largeurTexte = 0;
    int hauteurTexte = 0;

    if (TTF_SizeText(police, texte.c_str(), &largeurTexte, &hauteurTexte) != 0)
        return;

    int x = static_cast<int>((WINDOW_NES_WIDTH - largeurTexte) / 2);

    afficherTexte(texte, x, y);
}

/**
 * @brief Retourne le nom du personnage affiché dans le HUD.
 */
string GameGraphicMode::nomJoueur() const
{
    if (joueur == nullptr)
        return "JOUEUR";

    if (joueur->getPersonnage() == Personnage::luigi)
        return "LUIGI";

    return "MARIO";
}

/**
 * @brief Retourne le nom du niveau affiché dans le HUD.
 */
string GameGraphicMode::nomNiveau() const
{
    if (niveau == nullptr)
        return "NIVEAU ?";

    unsigned int id = niveau->getId();

    if (id == 4)
        return "NIVEAU 4";

    if (id == 5)
        return "NIVEAU 5";

    return "NIVEAU " + to_string(id);
}

/**
 * @brief Retourne l'état actuel du joueur.
 */
string GameGraphicMode::etatJoueur() const
{
    if (joueur == nullptr)
        return "INCONNU";

    if (joueur->getType() == TypeJoueur::petit)
        return "PETIT";

    if (joueur->getType() == TypeJoueur::grand)
        return "GRAND";

    if (joueur->getType() == TypeJoueur::feu)
        return "FEU";

    return "INCONNU";
}

/**
 * @brief Calcule le score maximum possible du niveau.
 */
unsigned int GameGraphicMode::calculerScoreMax() const
{
    if (niveau == nullptr)
        return 0;

    unsigned int max = 0;

    const vector<Item>& items = niveau->getItemsInit();
    const vector<Ennemi>& ennemis = niveau->getEnnemisInit();

    for (unsigned int i = 0; i < items.size(); i++)
    {
        if (items[i].getType() == TypeItem::piece)
            max += 10;
        else
            max += 50;
    }

    max += static_cast<unsigned int>(ennemis.size()) * 100;

    return max;
}

/**
 * @brief Affiche les informations du joueur en haut de l'écran.
 */
/**
 * @brief Affiche les informations du joueur en haut de l'écran.
 */
void GameGraphicMode::afficherHUD()
{
    if (jeu == nullptr || joueur == nullptr || niveau == nullptr)
        return;

    int tempsRestant = static_cast<int>(jeu->getTemps());

    string ligne1 = nomJoueur() + "   VIES " + to_string(joueur->getVies());
    string ligne2 = nomNiveau() + "   TEMPS " + to_string(tempsRestant);
    string ligne3 = "PIECES " + to_string(jeu->getPieces()) +
                    "   SCORE " + to_string(jeu->getScore());
    string ligne4 = "ETAT " + etatJoueur();

    afficherTexte(ligne1, 4, 4);
    afficherTexte(ligne2, 4, 16);
    afficherTexte(ligne3, 4, 28);
    afficherTexte(ligne4, 4, 40);

      /*
     * L'étoile est affichée sur une ligne séparée pour éviter que
     * la police Mario dépasse de l'écran.
     */

    if (joueur->estInvincible())
    {
        int tempsEtoile = static_cast<int>(joueur->getTempsEtoile() + 0.99f);
        afficherTexte("ETOILE " + to_string(tempsEtoile) + "S", 4, 52);
    }
}






/**
 * @brief Affiche l'écran de pause.
 */
void GameGraphicMode::afficherPause()
{
    SDL_Renderer* r = contexte->getRenderer();

    SDL_Rect fond;
    fond.x = 35;
    fond.y = 70;
    fond.w = 186;
    fond.h = 92;

    SDL_SetRenderDrawColor(r, 0, 0, 0, 220);
    SDL_RenderFillRect(r, &fond);

    afficherTexteCentre("PAUSE", 88);
    afficherTexteCentre("P: CONTINUER", 116);
    afficherTexteCentre("ECHAP: QUITTER", 136);
}

/**
 * @brief Retourne le sprite actuel du joueur.
 */
const char* GameGraphicMode::spriteJoueur() const
{
    if (joueur == nullptr)
        return "mario_base.png";

    bool luigi = joueur->getPersonnage() == Personnage::luigi;
    bool gauche = joueur->getDir() == Direction::Gauche;
    bool saut = joueur->getVit().y < 0.f;
    bool marcheDroite = joueur->getVit().x > 0.f;
    bool marcheGauche = joueur->getVit().x < 0.f;

    unsigned int k = (frame / 8) % 3;

    /*
     * Mario / Luigi feu.
     */
    if (joueur->getType() == TypeJoueur::feu)
    {
        if (luigi)
        {
            if (saut)
                return gauche ? "luigi_feu_saut_gauche.png" : "luigi_feu_saut_droite.png";

            if (marcheDroite)
            {
                if (k == 0) return "luigi_feu_marche_droite_1.png";
                if (k == 1) return "luigi_feu_marche_droite_2.png";
                return "luigi_feu_marche_droite_3.png";
            }

            if (marcheGauche)
            {
                if (k == 0) return "luigi_feu_marche_gauche_1.png";
                if (k == 1) return "luigi_feu_marche_gauche_2.png";
                return "luigi_feu_marche_gauche_3.png";
            }

            return gauche ? "luigi_feu_initial_gauche.png" : "luigi_feu_initial.png";
        }

        if (saut)
            return gauche ? "mario_feu_saut_gauche.png" : "mario_feu_saut_droite.png";

        if (marcheDroite)
        {
            if (k == 0) return "mario_feu_marche_droite_1.png";
            if (k == 1) return "mario_feu_marche_droite_2.png";
            return "mario_feu_marche_droite_3.png";
        }

        if (marcheGauche)
        {
            if (k == 0) return "mario_feu_marche_gauche_1.png";
            if (k == 1) return "mario_feu_marche_gauche_2.png";
            return "mario_feu_marche_gauche_3.png";
        }

        return gauche ? "mario_feu_initial_gauche.png" : "mario_feu_initial.png";
    }

    /*
     * Grand Mario / Grand Luigi.
     */
    if (joueur->getType() == TypeJoueur::grand)
    {
        if (luigi)
        {
            if (saut)
                return gauche ? "grand_luigi_saut_gauche.png" : "grand_luigi_saut_droite.png";

            if (marcheDroite)
            {
                if (k == 0) return "grand_luigi_pas1_droite.png";
                if (k == 1) return "grand_luigi_pas2_droite.png";
                return "grand_luigi_pas3_droite.png";
            }

            if (marcheGauche)
            {
                if (k == 0) return "grand_luigi_pas1_gauche.png";
                if (k == 1) return "grand_luigi_pas2_gauche.png";
                return "grand_luigi_pas3_gauche.png";
            }

            return gauche ? "grand_luigi_base_gauche.png" : "grand_luigi_base.png";
        }

        if (saut)
            return gauche ? "mario_grand_saut_gauche.png" : "mario_grand_saut_droite.png";

        if (marcheDroite)
        {
            if (k == 0) return "mario_grand_marche_droite_1.png";
            if (k == 1) return "mario_grand_marche_droite_2.png";
            return "mario_grand_marche_droite_3.png";
        }

        if (marcheGauche)
        {
            if (k == 0) return "mario_grand_marche_gauche_1.png";
            if (k == 1) return "mario_grand_marche_gauche_2.png";
            return "mario_grand_marche_gauche_3.png";
        }

        return gauche ? "mario_grand_marche_gauche_1.png" : "mario_grand_initial.png";
    }

    /*
     * Petit Mario / Petit Luigi.
     */
    if (luigi)
    {
        if (saut)
            return gauche ? "luigi_saut_gauche.png" : "luigi_saut_droite.png";

        if (marcheDroite)
        {
            if (k == 0) return "luigi_droite_pas1.png";
            if (k == 1) return "luigi_droite_pas2.png";
            return "luigi_droite_pas3.png";
        }

        if (marcheGauche)
        {
            if (k == 0) return "luigi_droite_pas1_gauche.png";
            if (k == 1) return "luigi_droite_pas2_gauche.png";
            return "luigi_droite_pas3_gauche.png";
        }

        return gauche ? "luigi_base_gauche.png" : "luigi_base_droite.png";
    }

    if (saut)
        return gauche ? "mario_saut_gauche.png" : "mario_saut_droite.png";

    if (marcheDroite)
    {
        if (k == 0) return "mario_droite_pas1.png";
        if (k == 1) return "mario_droite_pas2.png";
        return "mario_droite_pas3.png";
    }

    if (marcheGauche)
    {
        if (k == 0) return "mario_gauche_pas1.png";
        if (k == 1) return "mario_gauche_pas2.png";
        return "mario_gauche_pas3.png";
    }

    return gauche ? "mario_base_gauche.png" : "mario_base.png";
}

/**
 * @brief Affiche la carte du niveau.
 */
void GameGraphicMode::afficherCarte()
{
    if (niveau == nullptr)
        return;

    for (unsigned int y = 0; y < niveau->getDimY(); y++)
    {
        for (unsigned int x = 0; x < niveau->getDimX(); x++)
        {
            TypeTuile t = niveau->getTuile(x, y);

            int px = static_cast<int>(x * 16);
            int py = static_cast<int>(y * 16);

            if (t == TypeTuile::sol)
            {
                afficherSprite("sol.png", px, py, 16, 16);
            }
            else if (t == TypeTuile::incassable)
            {
                afficherSprite("bloc_dur.png", px, py, 16, 16);
            }
            else if (t == TypeTuile::cassable)
            {
                afficherSprite("bloc_cassable.png", px, py, 16, 16);
            }
            else if (t == TypeTuile::tuyau)
            {
                afficherSprite("tuyau.png", px, py, 16, 16);
            }
            else if (t == TypeTuile::mystere)
            {
                unsigned int k = (frame / 12) % 3;

                if (k == 0)
                    afficherSprite("tuile_actif1.png", px, py, 16, 16);
                else if (k == 1)
                    afficherSprite("tuile_actif2.png", px, py, 16, 16);
                else
                    afficherSprite("tuile_actif3.png", px, py, 16, 16);
            }
        }
    }

    int fx = static_cast<int>(niveau->getFin().x);
    int fy = static_cast<int>(niveau->getFin().y);

    afficherSprite("drapeau_fin.png", fx, fy - 160, 16, 176);
}

/**
 * @brief Affiche les items du niveau.
 */
void GameGraphicMode::afficherItems()
{
    if (jeu == nullptr)
        return;

    const vector<Item>& items = jeu->getItems();

    for (unsigned int i = 0; i < items.size(); i++)
    {
        if (!items[i].estDisponible())
            continue;

        int x = static_cast<int>(items[i].getPos().x);
        int y = static_cast<int>(items[i].getPos().y);

        if (items[i].getType() == TypeItem::piece)
        {
            unsigned int k = (frame / 8) % 4;

            if (k == 0)
                afficherSprite("coin_1.png", x, y, 16, 16);
            else if (k == 1)
                afficherSprite("coin_2.png", x, y, 16, 16);
            else if (k == 2)
                afficherSprite("coin_3.png", x, y, 16, 16);
            else
                afficherSprite("coin_4.png", x, y, 16, 16);
        }
        else if (items[i].getType() == TypeItem::champignon)
        {
            afficherSprite("champignon.png", x, y, 16, 16);
        }
        else if (items[i].getType() == TypeItem::fleur)
        {
            afficherSprite("fleur_feu.png", x, y, 16, 16);
        }
        else if (items[i].getType() == TypeItem::etoile)
        {
            afficherSprite("etoile.png", x, y, 16, 16);
        }
    }
}

/**
 * @brief Affiche les ennemis du niveau.
 */
void GameGraphicMode::afficherEnnemis()
{
    if (jeu == nullptr)
        return;

    const vector<Ennemi>& ens = jeu->getEnnemis();

    for (unsigned int i = 0; i < ens.size(); i++)
    {
        if (!ens[i].estEnVie())
            continue;

        int x = static_cast<int>(ens[i].getPos().x);
        int y = static_cast<int>(ens[i].getPos().y);

        if (ens[i].getType() == TypeEnnemi::Goomba)
        {
            if ((frame / 12) % 2 == 0)
                afficherSprite("goomba_pas1.png", x, y, 16, 16);
            else
                afficherSprite("goomba_pas2.png", x, y, 16, 16);
        }
        else
        {
            if (ens[i].getDir() == Direction::Gauche)
            {
                if ((frame / 12) % 2 == 0)
                    afficherSprite("koopa_gauche_pas1.png", x, y, 16, 24);
                else
                    afficherSprite("koopa_gauche_pas2.png", x, y, 16, 24);
            }
            else
            {
                if ((frame / 12) % 2 == 0)
                    afficherSprite("koopa_droite_pas1.png", x, y, 16, 24);
                else
                    afficherSprite("koopa_droite_pas2.png", x, y, 16, 24);
            }
        }
    }
}

/**
 * @brief Affiche les boules de feu.
 */
void GameGraphicMode::afficherFeux()
{
    if (jeu == nullptr)
        return;

    const vector<Feu>& feux = jeu->getBoules();

    for (unsigned int i = 0; i < feux.size(); i++)
    {
        if (feux[i].estActive())
        {
            int x = static_cast<int>(feux[i].getPos().x);
            int y = static_cast<int>(feux[i].getPos().y);

            unsigned int k = (frame / 5) % 4;

            if (k == 0)
                afficherSprite("boule_feu_1.png", x, y, 8, 8);
            else if (k == 1)
                afficherSprite("boule_feu_2.png", x, y, 8, 8);
            else if (k == 2)
                afficherSprite("boule_feu_3.png", x, y, 8, 8);
            else
                afficherSprite("boule_feu_4.png", x, y, 8, 8);
        }
    }
}

/**
 * @brief Affiche le joueur.
 */
void GameGraphicMode::afficherJoueur()
{
    if (joueur == nullptr)
        return;

    /*
     * Clignotement :
     * - pendant l'étoile
     * - pendant la protection après un dégât
     */
    if ((joueur->estInvincible() || joueur->estProtege()) &&
        (frame / 5) % 2 == 0)
    {
        return;
    }

    int x = static_cast<int>(joueur->getPos().x);
    int y = static_cast<int>(joueur->getPos().y);

    int largeur = static_cast<int>(joueur->getLargeur());
    int hauteur = static_cast<int>(joueur->getHauteur());

    afficherSprite(spriteJoueur(), x, y, largeur, hauteur);
}

/**
 * @brief Affiche toute la scène.
 */
void GameGraphicMode::afficher()
{
    SDL_Renderer* r = contexte->getRenderer();

    majCamera();

    SDL_SetRenderDrawColor(r, 92, 148, 252, 255);
    SDL_RenderClear(r);

    afficherCarte();
    afficherItems();
    afficherEnnemis();
    afficherFeux();
    afficherJoueur();
    afficherHUD();

    if (enPause)
        afficherPause();

    SDL_RenderPresent(r);
}

/**
 * @brief Boucle principale du mode graphique.
 */
void GameGraphicMode::boucle()
{
    actif = true;
    enPause = false;

    while (actif)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                actif = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    actif = false;
                }

                if (event.key.keysym.sym == SDLK_p)
                {
                    enPause = !enPause;
                }
            }
        }

        /*
         * En pause, on garde l'affichage mais on ne met plus à jour :
         * joueur, ennemis, collisions, score et temps.
         */
        if (!enPause && jeu != nullptr)
        {
            EntreeJoueur entree = lireEntree();

            jeu->maj(entree, 1.f / 60.f);

            if (jeu->getEtat() != EtatPartie::enCours)
                actif = false;
        }

        afficher();

        if (!enPause)
            frame++;

        SDL_Delay(1000 / 60);
    }

    fermerPolice();
}