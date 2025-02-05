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
void afficher_jeu(int *running, Groupe *groupe, int *return_to_menu);

// Affichage d'un événement graphique
void afficher_evenement_sdl(Groupe *groupe, const char *image_path, const char *texte, 
                            int effet_choix1_nourriture, int effet_choix1_eau, 
                            int effet_choix2_nourriture, int effet_choix2_eau);
                            
// Affichage de la transition entre les jours
void afficher_transition_jour(SDL_Renderer *renderer, int jour);

// Affichage d'une barre de progression
void afficher_barre(SDL_Renderer *renderer, int x, int y, int largeur, int hauteur, int valeur, int max, SDL_Color couleur);

// Nettoyage des ressources SDL
void cleanup_renderer();

void afficher_ecran_titre(int *running);

#endif
