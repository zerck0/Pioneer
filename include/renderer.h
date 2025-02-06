#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"

int init_renderer();
void afficher_menu(int *running, int *start_game, Groupe *groupe, int *loaded_game);
void afficher_jeu(int *running, Groupe *groupe, int *return_to_menu);
void afficher_evenement_sdl(Groupe *groupe, const char *image_path, const char *texte, 
                            int effet_choix1_nourriture, int effet_choix1_eau, 
                            int effet_choix2_nourriture, int effet_choix2_eau,
                            int effet_choix1_colons, int effet_choix2_colons,
                            int effet_choix1_sante, int effet_choix2_sante,
                            int effet_choix1_fatigue, int effet_choix2_fatigue,
                            int effet_choix1_distance, int effet_choix2_distance);
void afficher_transition_jour(SDL_Renderer *renderer, int jour);
void afficher_barre(SDL_Renderer *renderer, int x, int y, int largeur, int hauteur, int valeur, int max, SDL_Color couleur);
void cleanup_renderer();
void afficher_ecran_titre(int *running);
void afficher_options_menu(int *running);
void toggle_fullscreen();
void toggle_music();
void afficher_ecran_game_over(int *running, int *return_to_menu);
int sdl_file_dialog(char *filepath, size_t filepath_size);
int confirm_exit();
void afficher_texte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, SDL_Color couleur);
void afficher_personnages_sdl(SDL_Renderer *renderer, TTF_Font *font, const Personnage *joueur, const Personnage *adversaire);
void afficher_sorts_sdl(SDL_Renderer *renderer, TTF_Font *font);

#endif
