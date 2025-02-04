#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"

// Initialisation du renderer
int init_renderer();

// Affichage du menu principal
void afficher_menu(int *running, int *start_game);

// Affichage du jeu avec interface complète
void afficher_jeu(int *running, Groupe *groupe);

// Affichage d'une transition "Jour +1"
void afficher_transition_jour(SDL_Renderer *renderer, int jour);

// Affichage d'une barre de progression (ex: santé, eau...)
void afficher_barre(SDL_Renderer *renderer, int x, int y, int largeur, int hauteur, int valeur, int max, SDL_Color couleur);

// Nettoyage des ressources SDL
void cleanup_renderer();

#endif