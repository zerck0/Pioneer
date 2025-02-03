#include "../include/renderer.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "../include/ui.h"
#include "../include/game.h"

// Dimensions de la fenêtre
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Variables SDL
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *background_texture = NULL;
SDL_Texture *btn_new_game_texture = NULL;
SDL_Texture *btn_options_texture = NULL;
SDL_Texture *btn_quit_texture = NULL;

// Rectangles des boutons
SDL_Rect btn_new_game, btn_options, btn_quit;

// Fonction pour charger une texture
SDL_Texture* load_texture(const char *file_path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, file_path);
    if (!texture) {
        printf("Erreur IMG_LoadTexture : %s\n", IMG_GetError());
    }
    return texture;
}

// Initialisation SDL
int init_renderer() {
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

    // Chargement des textures
    background_texture = load_texture("assets/images/menu.jpg");
    btn_new_game_texture = load_texture("assets/images/new.png");
    btn_options_texture = load_texture("assets/images/option.png");
    btn_quit_texture = load_texture("assets/images/quit.png");

    if (!background_texture || !btn_new_game_texture || !btn_options_texture || !btn_quit_texture) {
        return 1;
    }

    // Alignement horizontal des boutons
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

// Affichage du menu
void afficher_menu(int *running, int *start_game) {
    SDL_Event event;
    int in_menu = 1;

    while (in_menu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                in_menu = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= btn_new_game.x && x <= btn_new_game.x + btn_new_game.w &&
                    y >= btn_new_game.y && y <= btn_new_game.y + btn_new_game.h) {
                    printf("Nouvelle Partie sélectionnée !\n");
                    *start_game = 1;
                    in_menu = 0;
                } else if (x >= btn_options.x && x <= btn_options.x + btn_options.w &&
                           y >= btn_options.y && y <= btn_options.y + btn_options.h) {
                    printf("Options sélectionnées !\n");
                } else if (x >= btn_quit.x && x <= btn_quit.x + btn_quit.w &&
                           y >= btn_quit.y && y <= btn_quit.y + btn_quit.h) {
                    printf("Quitter sélectionné !\n");
                    *running = 0;
                    in_menu = 0;
                }
            }
        }

        // Rendu du menu
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        SDL_RenderCopy(renderer, btn_new_game_texture, NULL, &btn_new_game);
        SDL_RenderCopy(renderer, btn_options_texture, NULL, &btn_options);
        SDL_RenderCopy(renderer, btn_quit_texture, NULL, &btn_quit);
        SDL_RenderPresent(renderer);
    }
}

// Déclarations des textures des actions
SDL_Texture *icon_walk = NULL;
SDL_Texture *icon_water = NULL;
SDL_Texture *icon_food = NULL;
SDL_Texture *icon_rest = NULL;

void afficher_jeu(int *running, Groupe *groupe) {
    SDL_Event event;
    int in_game = 1;

    // Charger la police
    TTF_Font *font = TTF_OpenFont("assets/fonts/Opensans.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return;
    }

    SDL_Color blanc = {255, 255, 255};

    // Charger le fond du jeu
    SDL_Texture *background_texture = load_texture("assets/images/game_background.jpg");

    // Charger les icônes des actions
    SDL_Texture *icon_walk = load_texture("assets/icons/walk.png");
    SDL_Texture *icon_water = load_texture("assets/icons/drink.png");
    SDL_Texture *icon_food = load_texture("assets/icons/eat.png");
    SDL_Texture *icon_rest = load_texture("assets/icons/rest.png");

    // Zones de positionnement
    SDL_Rect rect_infos = {0, 0, 800, 100};    // Barre supérieure
    SDL_Rect rect_actions = {0, 500, 800, 100}; // Barre inférieure
    SDL_Rect rect_background = {0, 100, 800, 400}; // Fond de jeu

    // Positions des icônes
    SDL_Rect rect_walk = {100, 520, 50, 50};
    SDL_Rect rect_water = {200, 520, 50, 50};
    SDL_Rect rect_food = {300, 520, 50, 50};
    SDL_Rect rect_rest = {400, 520, 50, 50};

    while (in_game) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                in_game = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= rect_walk.x && x <= rect_walk.x + rect_walk.w && y >= rect_walk.y && y <= rect_walk.y + rect_walk.h) {
                    choix_joueur(groupe, 1);
                } else if (x >= rect_water.x && x <= rect_water.x + rect_water.w && y >= rect_water.y && y <= rect_water.y + rect_water.h) {
                    choix_joueur(groupe, 2);
                } else if (x >= rect_food.x && x <= rect_food.x + rect_food.w && y >= rect_food.y && y <= rect_food.y + rect_food.h) {
                    choix_joueur(groupe, 3);
                } else if (x >= rect_rest.x && x <= rect_rest.x + rect_rest.w && y >= rect_rest.y && y <= rect_rest.y + rect_rest.h) {
                    choix_joueur(groupe, 4);
                }
            }
        }

        // Effacer l’écran et dessiner le fond
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_texture, NULL, &rect_background);

        // Dessiner la barre d'infos
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &rect_infos);

        // Afficher les infos du groupe
        char buffer[256];

        sprintf(buffer, "Jour: %d | Distance: %d km", groupe->joursPasses, groupe->distanceRestante);
        afficher_texte(renderer, font, buffer, 20, 20, blanc);

        sprintf(buffer, "Colons: %d | Santé: %d", groupe->colons, groupe->sante);
        afficher_texte(renderer, font, buffer, 20, 50, blanc);

        sprintf(buffer, "Nourriture: %d | Eau: %d", groupe->nourriture, groupe->eau);
        afficher_texte(renderer, font, buffer, 400, 20, blanc);

        sprintf(buffer, "Fatigue: %d | Température: %d°C", groupe->fatigue, groupe->temperature);
        afficher_texte(renderer, font, buffer, 400, 50, blanc);

        // Dessiner la barre d'actions
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_RenderFillRect(renderer, &rect_actions);

        // Afficher les icônes des actions
        SDL_RenderCopy(renderer, icon_walk, NULL, &rect_walk);
        SDL_RenderCopy(renderer, icon_water, NULL, &rect_water);
        SDL_RenderCopy(renderer, icon_food, NULL, &rect_food);
        SDL_RenderCopy(renderer, icon_rest, NULL, &rect_rest);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyage des textures
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(icon_walk);
    SDL_DestroyTexture(icon_water);
    SDL_DestroyTexture(icon_food);
    SDL_DestroyTexture(icon_rest);
    TTF_CloseFont(font);
}


void cleanup_renderer() {
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(btn_new_game_texture);
    SDL_DestroyTexture(btn_options_texture);
    SDL_DestroyTexture(btn_quit_texture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}