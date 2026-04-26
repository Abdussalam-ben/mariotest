# Compilateur et options

CC = g++
DOXY = doxygen
CFLAGS = -std=c++11 -Wall -Wextra -Wconversion -pedantic -ggdb -finput-charset=UTF-8

# Répertoires

SRC_DIR = src
CORE_DIR = $(SRC_DIR)/core
TEXT_DIR = $(SRC_DIR)/text
OBJ_DIR = obj
BIN_DIR = bin
DOC_DIR = doc

# Cible principale

all: $(BIN_DIR)/JeuTexte

# Exécutable du mode texte

$(BIN_DIR)/JeuTexte: \
	$(OBJ_DIR)/JeuTexte.o \
	$(OBJ_DIR)/ModeTexte.o \
	$(OBJ_DIR)/Jeu.o \
	$(OBJ_DIR)/Joueur.o \
	$(OBJ_DIR)/Niveau.o \
	$(OBJ_DIR)/Vec2.o \
	$(OBJ_DIR)/Entity.o \
	$(OBJ_DIR)/Ennemi.o \
	$(OBJ_DIR)/Item.o \
	$(OBJ_DIR)/Feu.o \
	$(OBJ_DIR)/PlateformeMobile.o \
	$(OBJ_DIR)/Tuile.o \
	$(OBJ_DIR)/EntreeJoueur.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Fichiers texte

$(OBJ_DIR)/JeuTexte.o: $(TEXT_DIR)/JeuTexte.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/ModeTexte.o: $(TEXT_DIR)/ModeTexte.cpp $(TEXT_DIR)/ModeTexte.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Fichiers core

$(OBJ_DIR)/Jeu.o: $(CORE_DIR)/Jeu.cpp $(CORE_DIR)/Jeu.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Joueur.o: $(CORE_DIR)/Joueur.cpp $(CORE_DIR)/Joueur.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Niveau.o: $(CORE_DIR)/Niveau.cpp $(CORE_DIR)/Niveau.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Vec2.o: $(CORE_DIR)/Vec2.cpp $(CORE_DIR)/Vec2.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Entity.o: $(CORE_DIR)/Entity.cpp $(CORE_DIR)/Entity.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Ennemi.o: $(CORE_DIR)/Ennemi.cpp $(CORE_DIR)/Ennemi.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Item.o: $(CORE_DIR)/Item.cpp $(CORE_DIR)/Item.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Feu.o: $(CORE_DIR)/Feu.cpp $(CORE_DIR)/Feu.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/PlateformeMobile.o: $(CORE_DIR)/PlateformeMobile.cpp $(CORE_DIR)/PlateformeMobile.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/Tuile.o: $(CORE_DIR)/Tuile.cpp $(CORE_DIR)/Tuile.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/EntreeJoueur.o: $(CORE_DIR)/EntreeJoueur.cpp $(CORE_DIR)/EntreeJoueur.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création des dossiers

$(OBJ_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

# Lancer le mode texte

run: $(BIN_DIR)/JeuTexte
	./$(BIN_DIR)/JeuTexte

# Lancer le mode graphique

GRAPHIC_DIR = $(SRC_DIR)/graphic

graphic: $(BIN_DIR)/JeuGraphique

$(BIN_DIR)/JeuGraphique: \
	$(GRAPHIC_DIR)/GraphicGame.cpp \
	$(GRAPHIC_DIR)/GameGraphicMode.cpp \
	$(GRAPHIC_DIR)/SDLContext.cpp \
	$(GRAPHIC_DIR)/TextureManager.cpp \
	$(CORE_DIR)/Jeu.cpp \
	$(CORE_DIR)/Joueur.cpp \
	$(CORE_DIR)/Niveau.cpp \
	$(CORE_DIR)/Vec2.cpp \
	$(CORE_DIR)/Entity.cpp \
	$(CORE_DIR)/Ennemi.cpp \
	$(CORE_DIR)/Item.cpp \
	$(CORE_DIR)/Feu.cpp \
	$(CORE_DIR)/PlateformeMobile.cpp \
	$(CORE_DIR)/Tuile.cpp \
	$(CORE_DIR)/EntreeJoueur.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -Iextern/include $^ -Lextern/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -o $@s

# Documentation

docu:
	$(DOXY) $(DOC_DIR)/Doxyfile

# Nettoyage

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*
	rm -rf $(DOC_DIR)/html