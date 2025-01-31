#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "game.h"

// Initialisation du renderer
int init_renderer();

// Affichage du menu principal
void afficher_menu(int *running, int *start_game);

// Affichage du jeu (avec le groupe de colons)
void afficher_jeu(int *running, Groupe *groupe);

// Nettoyage des ressources
void cleanup_renderer();

#endif
