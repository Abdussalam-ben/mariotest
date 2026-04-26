# ============================================================
# Makefile Fedora Linux - Mario C++ / SDL2
# Text mode + Graphic mode + SDL2_image + SDL2_ttf
# ============================================================

CXX      := g++
DOXY     := doxygen

SRC_DIR  := src
CORE_DIR := $(SRC_DIR)/core
TEXT_DIR := $(SRC_DIR)/text
GFX_DIR  := $(SRC_DIR)/graphic

OBJ_DIR  := obj
BIN_DIR  := bin
DOC_DIR  := doc

CXXFLAGS := -std=c++11 -Wall -Wextra -Wconversion -pedantic -ggdb
CPPFLAGS := -I$(CORE_DIR) -I$(TEXT_DIR) -I$(GFX_DIR)

SDL_CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf)
SDL_LIBS   := -lSDL2_ttf -lSDL2_image -lSDL2

CORE_SRC := $(wildcard $(CORE_DIR)/*.cpp)
TEXT_SRC := $(TEXT_DIR)/JeuTexte.cpp $(TEXT_DIR)/ModeTexte.cpp
GFX_SRC  := $(GFX_DIR)/GraphicGame.cpp \
            $(GFX_DIR)/GameGraphicMode.cpp \
            $(GFX_DIR)/SDLContext.cpp \
            $(GFX_DIR)/TextureManager.cpp

CORE_OBJ := $(patsubst $(CORE_DIR)/%.cpp,$(OBJ_DIR)/core/%.o,$(CORE_SRC))
TEXT_OBJ := $(patsubst $(TEXT_DIR)/%.cpp,$(OBJ_DIR)/text/%.o,$(TEXT_SRC))
GFX_OBJ  := $(patsubst $(GFX_DIR)/%.cpp,$(OBJ_DIR)/graphic/%.o,$(GFX_SRC))

TEXT_EXE := $(BIN_DIR)/JeuTexte
GFX_EXE  := $(BIN_DIR)/JeuGraphique

.PHONY: all text graphic run run-text run-graphic clean veryclean docu help

all: text graphic

text: $(TEXT_EXE)

graphic: $(GFX_EXE)

$(TEXT_EXE): $(CORE_OBJ) $(TEXT_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(GFX_EXE): $(CORE_OBJ) $(GFX_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(SDL_LIBS)

$(OBJ_DIR)/core/%.o: $(CORE_DIR)/%.cpp | $(OBJ_DIR)/core
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/text/%.o: $(TEXT_DIR)/%.cpp | $(OBJ_DIR)/text
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/graphic/%.o: $(GFX_DIR)/%.cpp | $(OBJ_DIR)/graphic
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SDL_CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR)/core:
	mkdir -p $(OBJ_DIR)/core

$(OBJ_DIR)/text:
	mkdir -p $(OBJ_DIR)/text

$(OBJ_DIR)/graphic:
	mkdir -p $(OBJ_DIR)/graphic

run: run-text

run-text: $(TEXT_EXE)
	./$(TEXT_EXE)

run-graphic: $(GFX_EXE)
	./$(GFX_EXE)

docu:
	$(DOXY) $(DOC_DIR)/Doxyfile

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(TEXT_EXE) $(GFX_EXE)

veryclean: clean
	rm -rf $(DOC_DIR)/html

help:
	@echo "make              -> compile text + graphic"
	@echo "make text         -> compile only text mode"
	@echo "make graphic      -> compile only graphic mode"
	@echo "make run-text     -> compile and run text mode"
	@echo "make run-graphic  -> compile and run graphic mode"
	@echo "make clean        -> remove compiled files"