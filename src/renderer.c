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
SDL_Color vert = {0, 255, 0};
SDL_Color rouge = {255, 0, 0};
SDL_Color bleu = {0, 0, 255};
SDL_Color jaune = {255, 255, 0};

// Fonction pour afficher le jeu avec toutes les infos et les actions
void afficher_jeu(int *running, Groupe *groupe) {
    SDL_Event event;
    int in_game = 1;

    // Initialisation des actions du jour
    ActionsJour actions;
    init_actions_jour(&actions);

    // Charger la police
    TTF_Font *font = TTF_OpenFont("assets/fonts/Opensans.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return;
    }
    SDL_Color blanc = {255, 255, 255};

    // Charger les textures
    background_texture = load_texture("assets/images/game_background.jpg");
    icon_walk = load_texture("assets/icons/walk.png");
    icon_water = load_texture("assets/icons/drink.png");
    icon_food = load_texture("assets/icons/eat.png");
    icon_rest = load_texture("assets/icons/rest.png");

    // Définition des zones d'affichage
    SDL_Rect rect_background = {0, 100, 800, 400}; // Fond du jeu
    SDL_Rect rect_infos = {0, 0, 800, 120}; // Zone d'infos (augmentée)
    SDL_Rect rect_actions = {0, 500, 800, 100}; // Barre d'actions
    SDL_Rect rect_fin_jour = {600, 520, 150, 50}; // Bouton "Fin du jour"

    // Taille des icônes et encadré noir
    int icon_size = 50;  
    int icon_border = 4;  
    SDL_Color noir = {0, 0, 0, 255};  

    // Positions des icônes d'action
    SDL_Rect rect_walk = {100, 520, icon_size, icon_size};
    SDL_Rect rect_water = {200, 520, icon_size, icon_size};
    SDL_Rect rect_food = {300, 520, icon_size, icon_size};
    SDL_Rect rect_rest = {400, 520, icon_size, icon_size};

    while (in_game) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                in_game = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                // Sélection des actions du jour
                if (x >= rect_walk.x && x <= rect_walk.x + rect_walk.w && y >= rect_walk.y && y <= rect_walk.y + rect_walk.h) {
                    actions.avancer = 1;
                } else if (x >= rect_water.x && x <= rect_water.x + rect_water.w && y >= rect_water.y && y <= rect_water.y + rect_water.h) {
                    actions.boire = 1;
                } else if (x >= rect_food.x && x <= rect_food.x + rect_food.w && y >= rect_food.y && y <= rect_food.y + rect_food.h) {
                    actions.manger = 1;
                } else if (x >= rect_rest.x && x <= rect_rest.x + rect_rest.w && y >= rect_rest.y && y <= rect_rest.y + rect_rest.h) {
                    actions.se_reposer = 1;
                }

                // Bouton "Fin du jour"
                if (x >= rect_fin_jour.x && x <= rect_fin_jour.x + rect_fin_jour.w &&
                    y >= rect_fin_jour.y && y <= rect_fin_jour.y + rect_fin_jour.h) {
                    afficher_transition_jour(renderer, groupe->joursPasses + 1);
                    appliquer_actions(groupe, &actions);
                }
            }
        }

        // Effacer l’écran et afficher le fond
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_texture, NULL, &rect_background);

        // Dessiner la barre d'infos
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &rect_infos);

        // Affichage des stats sous forme de texte
        char buffer[256];
        sprintf(buffer, "Jour: %d | Distance: %d km", groupe->joursPasses, groupe->distanceRestante);
        afficher_texte(renderer, font, buffer, 20, 20, blanc);

        // Définition des positions des barres
        int texte_x_gauche = 20;
        int barre_x_gauche = 200;
        int texte_x_droite = 430;
        int barre_x_droite = 570;
        int barre_largeur = 200;
        int barre_hauteur = 12;

        // Affichage des barres alignées avec le texte
        sprintf(buffer, "Santé: %d", groupe->sante);
        afficher_texte(renderer, font, buffer, texte_x_gauche, 50, blanc);
        afficher_barre(renderer, barre_x_gauche, 60, barre_largeur, barre_hauteur, groupe->sante, 100, vert);

        sprintf(buffer, "Fatigue: %d", groupe->fatigue);
        afficher_texte(renderer, font, buffer, texte_x_droite, 50, blanc);
        afficher_barre(renderer, barre_x_droite, 60, barre_largeur, barre_hauteur, groupe->fatigue, 100, rouge);

        sprintf(buffer, "Nourriture: %d", groupe->nourriture);
        afficher_texte(renderer, font, buffer, texte_x_gauche, 80, blanc);
        afficher_barre(renderer, barre_x_gauche, 90, barre_largeur, barre_hauteur, groupe->nourriture, 100, jaune);

        sprintf(buffer, "Eau: %d", groupe->eau);
        afficher_texte(renderer, font, buffer, texte_x_droite, 80, blanc);
        afficher_barre(renderer, barre_x_droite, 90, barre_largeur, barre_hauteur, groupe->eau, 100, bleu);

        // Dessiner la barre d'actions (bas)
        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_RenderFillRect(renderer, &rect_actions);

        // Effet de sélection des icônes (cadre noir)
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (actions.avancer) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_walk.x - icon_border, rect_walk.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.boire) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_water.x - icon_border, rect_water.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.manger) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_food.x - icon_border, rect_food.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.se_reposer) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_rest.x - icon_border, rect_rest.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});

        // Afficher les icônes des actions
        SDL_RenderCopy(renderer, icon_walk, NULL, &rect_walk);
        SDL_RenderCopy(renderer, icon_water, NULL, &rect_water);
        SDL_RenderCopy(renderer, icon_food, NULL, &rect_food);
        SDL_RenderCopy(renderer, icon_rest, NULL, &rect_rest);

        // Afficher le bouton "Fin du jour"
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &rect_fin_jour);
        afficher_texte(renderer, font, "Fin du jour", rect_fin_jour.x + 10, rect_fin_jour.y + 10, blanc);

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

void afficher_transition_jour(SDL_Renderer *renderer, int jour) {
    SDL_Color noir = {0, 0, 0};
    TTF_Font *font = TTF_OpenFont("assets/fonts/Opensans.ttf", 48);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return;
    }

    // Charger l'image de transition
    SDL_Texture *transition_texture = load_texture("assets/images/transition.jpg");

    char buffer[32];
    sprintf(buffer, "Jour %d", jour);

    // Afficher le background de transition
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, transition_texture, NULL, NULL);
    afficher_texte(renderer, font, buffer, 600, 400, noir);
    
    SDL_RenderPresent(renderer);
    SDL_Delay(2000); // Pause de 2 secondes avant de continuer

    // Nettoyage
    SDL_DestroyTexture(transition_texture);
    TTF_CloseFont(font);
}

void afficher_barre(SDL_Renderer *renderer, int x, int y, int largeur, int hauteur, int valeur, int max, SDL_Color couleur) {
    // Calcul de la largeur de la barre selon la valeur
    int barre_largeur = (valeur * largeur) / max;

    // Rectangle de fond (barre grise)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect fond = {x, y, largeur, hauteur};
    SDL_RenderFillRect(renderer, &fond);

    // Rectangle de la valeur actuelle (barre colorée)
    SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, 255);
    SDL_Rect barre = {x, y, barre_largeur, hauteur};
    SDL_RenderFillRect(renderer, &barre);
}
