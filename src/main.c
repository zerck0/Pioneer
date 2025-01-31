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
#include <windows.h>
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

int main(int argc, char *argv[]) {
    // Initialisation du rendu SDL
    if (init_renderer() != 0) {
        return 1;
    }

    int running = 1;
    int start_game = 0; // 0 = menu, 1 = jeu

    Groupe groupe;
    init_groupe(&groupe); // Initialisation du groupe de colons

    while (running) {
        if (!start_game) {
            afficher_menu(&running, &start_game);
        } else {
            afficher_jeu(&running, &groupe);
        }
    }

    cleanup_renderer();
    return 0;
}