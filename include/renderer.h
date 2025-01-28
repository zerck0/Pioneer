#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Initialisation du renderer
int init_renderer();

// Affichage des éléments à l'écran
void render();

// Gestion des événements (clics, etc.)
void handle_events(SDL_Event *event, int *running);

// Nettoyage des ressources
void cleanup_renderer();

#endif
