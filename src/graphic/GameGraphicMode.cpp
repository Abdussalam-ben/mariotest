#include "GameGraphicMode.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

GameGraphicMode::GameGraphicMode(SDLContext& c, TextureManager& t)
    : contexte(&c),
      textures(&t),
      jeu(nullptr),
      niveau(nullptr),
      joueur(nullptr),
      actif(true),
      frame(0),
      cameraX(0.f),
      cameraY(0.f),
      police(nullptr),
      scoreMax(0)
{
    initialiserPolice();
}

void GameGraphicMode::setJeu(Jeu& j, Niveau& n, Joueur& p)
{
    jeu = &j;
    niveau = &n;
    joueur = &p;

    scoreMax = calculerScoreMax();
}

EntreeJoueur GameGraphicMode::lireEntree() const
{
    EntreeJoueur entree;

    const Uint8* clavier = SDL_GetKeyboardState(NULL);

    entree.gauche = clavier[SDL_SCANCODE_Q] || clavier[SDL_SCANCODE_LEFT];
    entree.droite = clavier[SDL_SCANCODE_D] || clavier[SDL_SCANCODE_RIGHT];
    entree.saut = clavier[SDL_SCANCODE_SPACE] || clavier[SDL_SCANCODE_Z] || clavier[SDL_SCANCODE_UP];
    entree.tir = clavier[SDL_SCANCODE_F];

    return entree;
}

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

void GameGraphicMode::initialiserPolice()
{
    if (TTF_Init() == -1)
    {
        cerr << "Erreur TTF_Init : " << TTF_GetError() << endl;
        police = nullptr;
        return;
    }

    const char* chemins[] = {
        "/usr/share/fonts/dejavu-sans-fonts/DejaVuSerif.ttf",
        "/usr/share/fonts/dejavu/DejaVuSerif.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSerif.ttf",
        "/usr/share/fonts/dejavu-sans-fonts/DejaVuSans.ttf",
        "/usr/share/fonts/dejavu/DejaVuSans.ttf"
    };

    for (unsigned int i = 0; i < 5 && police == nullptr; i++)
    {
        police = TTF_OpenFont(chemins[i], 8);
    }

    if (police == nullptr)
    {
        cerr << "Erreur : impossible de charger une police." << endl;
        cerr << "Installe : sudo dnf install dejavu-sans-fonts" << endl;
    }
}

void GameGraphicMode::fermerPolice()
{
    if (police != nullptr)
    {
        TTF_CloseFont(police);
        police = nullptr;
    }

    TTF_Quit();
}

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

string GameGraphicMode::nomJoueur() const
{
    if (joueur == nullptr)
        return "Player";

    if (joueur->getPersonnage() == Personnage::luigi)
        return "Luigi";

    return "Mario";
}

string GameGraphicMode::nomNiveau() const
{
    if (niveau == nullptr)
        return "Niveau ?";

    unsigned int id = niveau->getId();

    if (id == 4)
        return "Niveau 4 difficile";

    if (id == 5)
        return "Niveau 5 long";

    return "Niveau " + to_string(id);
}

string GameGraphicMode::etatJoueur() const
{
    if (joueur == nullptr)
        return "inconnu";

    if (joueur->getType() == TypeJoueur::petit)
        return "Petit";

    if (joueur->getType() == TypeJoueur::grand)
        return "Grand";

    if (joueur->getType() == TypeJoueur::feu)
        return "Feu";

    if (joueur->getType() == TypeJoueur::etoile)
        return "Etoile";

    return "inconnu";
}

unsigned int GameGraphicMode::calculerScoreMax() const
{
    if (niveau == nullptr)
        return 0;

    unsigned int max = 0;

    max += static_cast<unsigned int>(niveau->getItemsInit().size()) * 10;
    max += static_cast<unsigned int>(niveau->getEnnemisInit().size()) * 100;

    return max;
}

void GameGraphicMode::afficherHUD()
{
    if (jeu == nullptr || joueur == nullptr || niveau == nullptr)
        return;

    int tempsRestant = static_cast<int>(jeu->getTemps());

    string ligne1 =
        nomJoueur() +
        " | Vies: " + to_string(joueur->getVies()) +
        " | " + nomNiveau();

    string ligne2 =
        "Temps: " + to_string(tempsRestant) +
        " | Etat: " + etatJoueur() +
        " | Score: " + to_string(jeu->getScore()) +
        " / " + to_string(scoreMax);

    afficherTexte(ligne1, 4, 4);
    afficherTexte(ligne2, 4, 16);
}

const char* GameGraphicMode::spriteJoueur() const
{
    if (joueur == nullptr)
        return "mario_base.png";

    if (joueur->getVit().y < 0.f)
    {
        if (joueur->getDir() == Direction::Gauche)
            return "mario_saut_gauche.png";

        return "mario_saut_droite.png";
    }

    if (joueur->getVit().x > 0.f)
    {
        unsigned int k = (frame / 8) % 3;

        if (k == 0)
            return "mario_droite_pas1.png";

        if (k == 1)
            return "mario_droite_pas2.png";

        return "mario_droite_pas3.png";
    }

    if (joueur->getVit().x < 0.f)
    {
        unsigned int k = (frame / 8) % 3;

        if (k == 0)
            return "mario_gauche_pas1.png";

        if (k == 1)
            return "mario_gauche_pas2.png";

        return "mario_gauche_pas3.png";
    }

    return "mario_base.png";
}

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

            if (t == TypeTuile::sol || t == TypeTuile::incassable)
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

    afficherSprite("tuile_vide.png", fx, fy, 16, 16);
}

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
            afficherSprite("piece.png", x, y, 16, 16);
        else if (items[i].getType() == TypeItem::champignon)
            afficherSprite("champignon.png", x, y, 16, 16);
        else if (items[i].getType() == TypeItem::fleur)
            afficherSprite("fleur_feu.png", x, y, 16, 16);
    }
}

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

            afficherSprite("fleur_feu.png", x, y, 12, 12);
        }
    }
}

void GameGraphicMode::afficherJoueur()
{
    if (joueur == nullptr)
        return;

    int x = static_cast<int>(joueur->getPos().x);
    int y = static_cast<int>(joueur->getPos().y);

    afficherSprite(spriteJoueur(), x, y, 16, 16);
}

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

    SDL_RenderPresent(r);
}

void GameGraphicMode::boucle()
{
    actif = true;

    while (actif)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                actif = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                actif = false;
        }

        if (jeu != nullptr)
        {
            EntreeJoueur entree = lireEntree();
            jeu->maj(entree, 1.f / 60.f);
        }

        afficher();

        frame++;

        SDL_Delay(1000 / 60);
    }

    fermerPolice();
}