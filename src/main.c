#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/groupe.h" 
#include "../include/game.h" 
#include "../include/renderer.h"

#ifdef _WIN32
#include <SDL2/SDL_image.h>
#else
#include <SDL2/SDL2_image/SDL_image.h>
#endif

int main(int argc, char *argv[]) {
    int running = 1;
    int start_game = 0;
    int return_to_menu = 0;

    if (init_renderer() != 0) {
        printf("Erreur lors de l'initialisation du renderer\n");
        return 1;
    }

    // Afficher l'écran titre
    afficher_ecran_titre(&running);

    while (running) {
        // Afficher l'écran titre avant le menu principal si retour au menu
        if (return_to_menu) {
            afficher_ecran_titre(&running);
            return_to_menu = 0;
        }

        // Afficher le menu principal
        afficher_menu(&running, &start_game);

        if (start_game) {
            Groupe groupe;
            init_groupe(&groupe);

            while (running && !return_to_menu) {
                afficher_jeu(&running, &groupe, &return_to_menu);
            }

            if (return_to_menu) {
                start_game = 0;
            }
        }
    }

    cleanup_renderer();
    return 0;
}