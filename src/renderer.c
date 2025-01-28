#include "../include/renderer.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "../include/ui.h"

// Dimensions de la fenêtre
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Variables SDL globales
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

// Textures globales
SDL_Texture *background_texture = NULL;
SDL_Texture *btn_new_game_texture = NULL;
SDL_Texture *btn_options_texture = NULL;
SDL_Texture *btn_quit_texture = NULL;

// Rectangles pour les boutons
SDL_Rect btn_new_game, btn_options, btn_quit;

// Fonction pour charger une texture depuis un fichier
SDL_Texture* load_texture(const char *file_path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, file_path);
    if (!texture) {
        printf("Erreur IMG_LoadTexture : %s\n", IMG_GetError());
    }
    return texture;
}

int init_renderer() {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        printf("Erreur TTF_Init : %s\n", TTF_GetError());
        return 1;
    }
    if (IMG_Init(IMG_INIT_PNG) == 0) {
        printf("Erreur IMG_Init : %s\n", IMG_GetError());
        return 1;
    }

    // Création de la fenêtre
    window = SDL_CreateWindow("Pioneer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        return 1;
    }

    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        return 1;
    }

    // Chargement de l'image de fond
    background_texture = load_texture("assets/images/menu.jpg");
    if (!background_texture) {
        return 1;
    }

    // Chargement des images des boutons
    btn_new_game_texture = load_texture("assets/images/new.png");
    btn_options_texture = load_texture("assets/images/option.png");
    btn_quit_texture = load_texture("assets/images/quit.png");

    if (!btn_new_game_texture || !btn_options_texture || !btn_quit_texture) {
        return 1;
    }

    // Initialisation des rectangles des boutons (alignés horizontalement)
    int button_width = 200;
    int button_height = 400;
    int spacing = 50;

    btn_new_game.w = button_width;
    btn_new_game.h = button_height;
    btn_new_game.x = (WINDOW_WIDTH / 2) - (1.5 * button_width) - spacing;
    btn_new_game.y = WINDOW_HEIGHT / 2 - button_height / 2;

    btn_options.w = button_width;
    btn_options.h = button_height;
    btn_options.x = (WINDOW_WIDTH / 2) - (button_width / 2);
    btn_options.y = WINDOW_HEIGHT / 2 - button_height / 2;

    btn_quit.w = button_width;
    btn_quit.h = button_height;
    btn_quit.x = (WINDOW_WIDTH / 2) + (button_width / 2) + spacing;
    btn_quit.y = WINDOW_HEIGHT / 2 - button_height / 2;

    return 0;
}



void render() {
    // Efface l'écran
    SDL_RenderClear(renderer);

    // Affiche l'image de fond
    SDL_RenderCopy(renderer, background_texture, NULL, NULL);

    // Affiche les boutons
    SDL_RenderCopy(renderer, btn_new_game_texture, NULL, &btn_new_game);
    SDL_RenderCopy(renderer, btn_options_texture, NULL, &btn_options);
    SDL_RenderCopy(renderer, btn_quit_texture, NULL, &btn_quit);

    // Présente le rendu
    SDL_RenderPresent(renderer);
}


void handle_events(SDL_Event *event, int *running) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        int x = event->button.x;
        int y = event->button.y;

        // Vérification des clics sur les boutons
        if (x >= btn_new_game.x && x <= btn_new_game.x + btn_new_game.w &&
            y >= btn_new_game.y && y <= btn_new_game.y + btn_new_game.h) {
            printf("Nouvelle Partie sélectionnée !\n");
        } else if (x >= btn_options.x && x <= btn_options.x + btn_options.w &&
                   y >= btn_options.y && y <= btn_options.y + btn_options.h) {
            printf("Options sélectionnées !\n");
        } else if (x >= btn_quit.x && x <= btn_quit.x + btn_quit.w &&
                   y >= btn_quit.y && y <= btn_quit.y + btn_quit.h) {
            printf("Quitter sélectionné !\n");
            *running = 0;
        }
    }
}


void cleanup_renderer() {
    // Libération des textures
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(btn_new_game_texture);
    SDL_DestroyTexture(btn_options_texture);
    SDL_DestroyTexture(btn_quit_texture);

    // Libération des ressources SDL
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}