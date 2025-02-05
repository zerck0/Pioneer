#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../include/game.h" // Include game.h to define Groupe

// Déclarations des fonctions
int init_renderer();
void afficher_menu(int *running, int *start_game);
void afficher_jeu(int *running, Groupe *groupe, int *return_to_menu); // Update this line
void cleanup_renderer();
void afficher_transition_jour(SDL_Renderer *renderer, int jour);
void afficher_barre(SDL_Renderer *renderer, int x, int y, int largeur, int hauteur, int valeur, int max, SDL_Color couleur);
void afficher_ecran_titre(int *running);
void afficher_ecran_game_over(int *running, int *return_to_menu); // Add this line

#endif // RENDERER_H