# Super Mario - Projet SDL2


## Informations factuelles

Nom : BENLASHEHAR
Prénom : Abdussalam
Numéro étudiant : p2207174
Lien du dépôt : https://forge.univ-lyon1.fr/p2207174/supermario-2
Projet réalisé individuellement.

## Présentation

Ce projet est un jeu de plateforme inspiré de Super Mario, développé en C++ avec la bibliothèque SDL2.

Le jeu permet de choisir un personnage, Mario ou Luigi, puis de sélectionner un niveau. Le joueur doit traverser le niveau, éviter ou éliminer les ennemis, collecter des objets, survivre aux obstacles et atteindre le drapeau de fin pour gagner.

Le projet contient un mode graphique complet avec sprites, menu, HUD, musique de fond, écran de pause, écran de victoire et écran de défaite.



## Fonctionnalités principales

- Menu principal avec choix du personnage
- Sélection du niveau
- Écran des commandes
- Personnages jouables : Mario et Luigi
- Plusieurs états du joueur :
  - Petit
  - Grand
  - Feu
  - Invincible avec l’étoile
- Système de vies
- Système de score
- Compteur de pièces
- Temps limité
- HUD en jeu
- Pause avec la touche `P`
- Musique de fond en boucle
- Animation des sprites
- Ennemis :
  - Goomba
  - Koopa
- Objets :
  - Pièce
  - Champignon
  - Fleur de feu
  - Étoile
- Boules de feu
- Blocs cassables
- Tuiles mystère
- Plateformes mobiles
- Drapeau de fin
- Écran de victoire
- Écran de défaite



## Commandes

| Touche | Action |
|-------|--------|
| `Q` ou flèche gauche | Aller à gauche |
| `D` ou flèche droite | Aller à droite |
| `Espace`, `Z` ou flèche haut | Sauter |
| `F` | Tirer une boule de feu |
| `P` | Mettre le jeu en pause / reprendre |
| `Entrée` ou `Espace` | Valider dans le menu |
| `Échap` | Quitter |



## Structure du projet


.
├── assets
│   ├── musiques
│   │   └── musique_mario.mp3
│   ├── niveaux
│   │   ├── level0.txt
│   │   ├── level1.txt
│   │   ├── level2.txt
│   │   ├── level3.txt
│   │   ├── level4_difficile.txt
│   │   └── level5_long.txt
│   ├── polices
│   │   └── super_mario_bros_nes.ttf
│   └── sprites
│       └── sprites du jeu
│
├── src
│   ├── core
│   │   ├── Jeu.cpp / Jeu.h
│   │   ├── Joueur.cpp / Joueur.h
│   │   ├── Niveau.cpp / Niveau.h
│   │   ├── Ennemi.cpp / Ennemi.h
│   │   ├── Item.cpp / Item.h
│   │   ├── Feu.cpp / Feu.h
│   │   ├── Hitbox.cpp / Hitbox.h
│   │   ├── Vec2.cpp / Vec2.h
│   │   └── Constantes.h
│   │
│   ├── graphic
│   │   ├── GraphicGame.cpp
│   │   ├── GameGraphicMode.cpp / GameGraphicMode.h
│   │   ├── SDLContext.cpp / SDLContext.h
│   │   └── TextureManager.cpp / TextureManager.h
│   │
│   └── text
│       └── fichiers éventuels du mode texte
│
├── obj
├── bin
├── makefile
└── README.md
`



## Prérequis

Pour compiler et exécuter le projet, il faut installer :

* un compilateur C++
* `make`
* SDL2
* SDL2_image
* SDL2_ttf
* SDL2_mixer

### Installation sur Fedora


sudo dnf install gcc-c++ make SDL2 SDL2-devel SDL2_image SDL2_image-devel SDL2_ttf SDL2_ttf-devel SDL2_mixer SDL2_mixer-devel


### Installation sur Ubuntu / Debian


sudo apt update
sudo apt install g++ make libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev




## Compilation

Depuis la racine du projet :

make clean
make



## Exécution

Toujours lancer le jeu depuis la racine du projet pour que les chemins vers les assets fonctionnent correctement.


./bin/JeuGraphique




## Important

Le jeu utilise des chemins relatifs vers les dossiers `assets`.

Il faut donc éviter de lancer l’exécutable directement depuis le dossier `bin`, sinon certains fichiers comme les sprites, la police ou la musique peuvent ne pas être trouvés.

Correct :


./bin/JeuGraphique


Incorrect :


cd bin
./JeuGraphique


## Description du gameplay

Le joueur commence avec plusieurs vies et doit atteindre le drapeau situé à la fin du niveau.

Pendant le jeu, il peut collecter différents objets :

* Les pièces augmentent le compteur de pièces et le score.
* Le champignon transforme le joueur en grand personnage.
* La fleur permet de tirer des boules de feu.
* L’étoile rend le joueur temporairement invincible.

Les ennemis peuvent être éliminés en leur sautant dessus, avec une boule de feu, ou pendant l’invincibilité donnée par l’étoile.

Si le joueur perd toutes ses vies ou si le temps arrive à zéro, la partie est perdue.



## HUD

Le HUD affiche les informations principales pendant la partie :


MARIO   VIES 3
NIVEAU 1   TEMPS 287
PIECES 2   SCORE 120
ETAT PETIT


Lorsque le joueur récupère une étoile, un compte à rebours apparaît :


ETOILE 5S



## Niveaux

Les niveaux sont stockés dans le dossier :


assets/niveaux


Chaque niveau est représenté par un fichier texte. Les caractères du fichier permettent de placer les éléments du niveau.

Exemples de caractères utilisés :

| Caractère | Élément            |
| --------- | ------------------ |
| `#`       | Bloc incassable    |
| `B`       | Bloc cassable      |
| `T`       | Tuyau              |
| `?`       | Tuile mystère      |
| `S`       | Position de départ |
| `F`       | Drapeau de fin     |
| `E`       | Goomba             |
| `K`       | Koopa              |
| `C`       | Pièce              |
| `M`       | Champignon         |
| `L`       | Fleur de feu       |
| `A`       | Étoile             |
| `P`       | Plateforme mobile  |



## Assets

Les ressources du jeu sont séparées en plusieurs dossiers :


assets/sprites
assets/niveaux
assets/polices
assets/musiques


La police utilisée dans le menu et le HUD est :


assets/polices/super_mario_bros_nes.ttf


La musique de fond utilisée est :


assets/musiques/musique_mario.mp3




## Organisation du code

Le projet est séparé en deux grandes parties.

### Partie core

La partie `core` contient la logique du jeu :

* déplacement du joueur
* collisions
* ennemis
* objets
* score
* vies
* temps
* état de la partie
* niveaux

### Partie graphic

La partie `graphic` contient l’affichage SDL2 :

* fenêtre SDL
* renderer
* chargement des textures
* affichage des sprites
* menu
* HUD
* pause
* écrans de fin
* police
* musique



## Auteur

Projet réalisé par :


P2207174 - BENLASHEHAR ABDUSSALAM


## Technologies utilisées

* C++
* SDL2
* SDL2_image
* SDL2_ttf
* SDL2_mixer
* Makefile



## Objectif du projet

L’objectif de ce projet est de réaliser un jeu de plateforme complet en C++ en utilisant SDL2, avec une séparation claire entre la logique du jeu et l’affichage graphique.

Le projet met en pratique :

* la programmation orientée objet
* la gestion des collisions
* l’affichage graphique avec SDL2
* la gestion des entrées clavier
* le chargement de ressources externes
* l’organisation d’un projet C++
* l’utilisation d’un Makefile


