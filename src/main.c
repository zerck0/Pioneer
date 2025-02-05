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
#endif

int main(int argc, char *argv[]) {
    int running = 1;
    int start_game = 0;
    int return_to_menu = 0;

    if (init_renderer() != 0) {
        printf("Erreur lors de l'initialisation du renderer\n");
        return 1;
    }

    // Afficher l'écran titre au démarrage
    printf("Démarrage du jeu...\n");
    afficher_ecran_titre(&running);

    while (running) {
        if (return_to_menu && !start_game) {
            printf("Retour à l'écran titre\n");
            afficher_ecran_titre(&running);
            return_to_menu = 0;
        }

        printf("Affichage du menu principal...\n");
        afficher_menu(&running, &start_game);

        if (start_game) {
            printf("Lancement du jeu...\n");
            Groupe groupe;
            init_groupe(&groupe);

            while (running && !return_to_menu) {
                afficher_jeu(&running, &groupe, &return_to_menu);
            }

            if (return_to_menu) {
                printf("Retour au menu principal...\n");
                start_game = 0;
                return_to_menu = 0;
            }
        }
    }

    cleanup_renderer();
    printf("Fermeture du jeu...\n");
    return 0;
}
