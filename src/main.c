#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/game.h" 
#include "../include/renderer.h"

#ifdef _WIN32
#include <SDL2/SDL_image.h>
#else
#endif

int main(int argc, char *argv[]) {
    int running = 1;
    int start_game = 0;
    int return_to_menu = 0;
    int loaded_game = 0;
    Groupe groupe;

    // Initialisation du renderer
    if (init_renderer() != 0) {
        printf("Erreur lors de l'initialisation du renderer\n");
        return 1;
    }

    // Affichage de l'écran titre
    afficher_ecran_titre(&running);

    while (running) {
        if (return_to_menu && !start_game) {
            afficher_ecran_titre(&running);
            return_to_menu = 0;
        }

        // Affichage du menu principal
        afficher_menu(&running, &start_game, &groupe, &loaded_game);

        if (start_game) {
            if (!loaded_game) {
                init_groupe(&groupe);
            }
            while (running && !return_to_menu) {
                afficher_jeu(&running, &groupe, &return_to_menu);
            }
            if (return_to_menu) {
                start_game = 0;
                return_to_menu = 0;
                loaded_game = 0;
            }
        }
    }

    // Nettoyage du renderer
    cleanup_renderer();
    return 0;
}