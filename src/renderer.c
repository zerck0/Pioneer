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

    // Recharger la texture du menu à chaque appel
    SDL_Texture *menu_background = load_texture("assets/images/menu.jpg");
    if (!menu_background) {
        printf("Erreur de chargement du background du menu\n");
        return;
    }

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
        SDL_RenderCopy(renderer, menu_background, NULL, NULL);
        SDL_RenderCopy(renderer, btn_new_game_texture, NULL, &btn_new_game);
        SDL_RenderCopy(renderer, btn_options_texture, NULL, &btn_options);
        SDL_RenderCopy(renderer, btn_quit_texture, NULL, &btn_quit);
        SDL_RenderPresent(renderer);
    }

    // Libération de la texture du menu après son utilisation
    SDL_DestroyTexture(menu_background);
}


// Déclarations des textures des actions
SDL_Texture *icon_walk = NULL;
SDL_Texture *icon_water = NULL;
SDL_Texture *icon_food = NULL;
SDL_Texture *icon_rest = NULL;
SDL_Texture *icon_hunt = NULL; // Icône pour Chasser
SDL_Texture *icon_fetch_water = NULL; // Icône pour Chercher de l'eau

SDL_Color vert = {0, 255, 0};
SDL_Color rouge = {255, 0, 0};
SDL_Color bleu = {0, 0, 255};
SDL_Color jaune = {255, 255, 0};

void afficher_ecran_game_over(int *running, int *return_to_menu) {
    SDL_Event event;
    int in_game_over_screen = 1;

    // Charger la texture de l'écran de game over
    SDL_Texture *game_over_texture = load_texture("assets/images/Game over.jpg");
    if (!game_over_texture) {
        printf("Erreur de chargement de l'image Game over.jpg\n");
        *running = 0;
        return;
    }

    while (in_game_over_screen) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                in_game_over_screen = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                in_game_over_screen = 0;
                *return_to_menu = 1;
            }
        }

        // Rendu de l'écran de game over
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, game_over_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(game_over_texture);
}

// Fonction pour afficher le jeu avec toutes les infos et les actions
void afficher_jeu(int *running, Groupe *groupe, int *return_to_menu) {
    SDL_Event event;
    int in_game = 1;

    // Initialisation des actions du jour
    ActionsJour actions;
    init_actions_jour(&actions);

    // Charger la police
    TTF_Font *font = TTF_OpenFont("assets/fonts/eurostile.ttf", 24);
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
    icon_hunt = load_texture("assets/icons/hunt.png");
    icon_fetch_water = load_texture("assets/icons/fetch_water.png");


    // Définition des zones d'affichage
    SDL_Rect rect_background = {0, 100, 800, 400}; // Fond du jeu
    SDL_Rect rect_infos = {0, 0, 800, 120}; // Zone d'infos (augmentée)
    SDL_Rect rect_actions = {0, 500, 800, 100}; // Barre d'actions
    SDL_Rect rect_fin_jour = {600, 520, 150, 50}; // Bouton "Fin du jour"
    


    // Taille des icônes et encadré noir
    int icon_size = 50;  
    int icon_border = 4;  

    // Positions des icônes d'action
    SDL_Rect rect_walk = {30, 520, icon_size, icon_size};
    SDL_Rect rect_water = {130, 520, icon_size, icon_size};
    SDL_Rect rect_food = {230, 520, icon_size, icon_size};
    SDL_Rect rect_rest = {330, 520, icon_size, icon_size};
    SDL_Rect rect_hunt = {430, 520, icon_size, icon_size}; // Position de l'icône "Chasser"
    SDL_Rect rect_fetch_water = {530, 520, icon_size, icon_size}; // Position de l'icône "Chercher de l'eau"

    int selected_actions = 0;

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
                    if (actions.avancer) {
                        actions.avancer = 0;
                        selected_actions--;
                    } else if (selected_actions < 3) {
                        actions.avancer = 1;
                        selected_actions++;
                    }
                } else if (x >= rect_water.x && x <= rect_water.x + rect_water.w && y >= rect_water.y && y <= rect_water.y + rect_water.h) {
                    if (actions.boire) {
                        actions.boire = 0;
                        selected_actions--;
                    } else if (selected_actions < 3) {
                        actions.boire = 1;
                        selected_actions++;
                    }
                } else if (x >= rect_food.x && x <= rect_food.x + rect_food.w && y >= rect_food.y && y <= rect_food.y + rect_food.h) {
                    if (actions.manger) {
                        actions.manger = 0;
                        selected_actions--;
                    } else if (selected_actions < 3) {
                        actions.manger = 1;
                        selected_actions++;
                    }
                } else if (x >= rect_rest.x && x <= rect_rest.x + rect_rest.w && y >= rect_rest.y && y <= rect_rest.y + rect_rest.h) {
                    if (actions.se_reposer) {
                        actions.se_reposer = 0;
                        selected_actions--;
                    } else if (selected_actions < 3) {
                        actions.se_reposer = 1;
                        selected_actions++;
                    }
                } else if (x >= rect_hunt.x && x <= rect_hunt.x + rect_hunt.w && y >= rect_hunt.y && y <= rect_hunt.y + rect_hunt.h) {  
                    if (actions.chasser) {  
                        actions.chasser = 0;  
                        selected_actions--;  
                    } else if (selected_actions < 3) {  
                        actions.chasser = 1;  
                        selected_actions++;  
                    }  
                } else if (x >= rect_fetch_water.x && x <= rect_fetch_water.x + rect_fetch_water.w && y >= rect_fetch_water.y && y <= rect_fetch_water.y + rect_fetch_water.h) {  
                    if (actions.chercher_eau) {  
                        actions.chercher_eau = 0;  
                        selected_actions--;  
                    } else if (selected_actions < 3) {  
                        actions.chercher_eau = 1;  
                        selected_actions++;  
                    }  
                }
   

                // Bouton "Fin du jour"
                if (x >= rect_fin_jour.x && x <= rect_fin_jour.x + rect_fin_jour.w &&
                    y >= rect_fin_jour.y && y <= rect_fin_jour.y + rect_fin_jour.h) {
                    afficher_transition_jour(renderer, groupe->joursPasses + 1);
                    appliquer_actions(groupe, &actions);
                    selected_actions = 0;

                    // Vérifier si la santé est à 0 ou en dessous
                    if (groupe->sante <= 0) {
                        printf("Perdu ! Votre colonie n'a pas survécue.\n");
                        afficher_ecran_game_over(running, return_to_menu);
                        in_game = 0;
                    }
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

        sprintf(buffer, "Colons: %d", groupe->colons);
        afficher_texte(renderer, font, buffer, texte_x_droite, 20, blanc);
        afficher_barre(renderer, barre_x_droite, 30, barre_largeur, barre_hauteur, groupe->colons, 100, (SDL_Color){255, 165, 0, 255});

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
        if (actions.chasser) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_hunt.x - icon_border, rect_hunt.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.chercher_eau) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_fetch_water.x - icon_border, rect_fetch_water.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});

        // Afficher les icônes des actions
        SDL_RenderCopy(renderer, icon_walk, NULL, &rect_walk);
        SDL_RenderCopy(renderer, icon_water, NULL, &rect_water);
        SDL_RenderCopy(renderer, icon_food, NULL, &rect_food);
        SDL_RenderCopy(renderer, icon_rest, NULL, &rect_rest);
        SDL_RenderCopy(renderer, icon_hunt, NULL, &rect_hunt);
        SDL_RenderCopy(renderer, icon_fetch_water, NULL, &rect_fetch_water);


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
    SDL_DestroyTexture(icon_hunt);
    SDL_DestroyTexture(icon_fetch_water);
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
    SDL_Color noir = {0, 0, 0, 255}; // Texte en noir
    TTF_Font *font = TTF_OpenFont("assets/fonts/Opensans.ttf", 48);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return;
    }

    // Charger l'image de transition
    SDL_Texture *transition_texture = load_texture("assets/images/transition.jpg");
    if (!transition_texture) {
        printf("Erreur : Impossible de charger l'image de transition\n");
        return;
    }

    char buffer[32];
    sprintf(buffer, "Jour %d", jour);

    // Afficher le background de transition
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, transition_texture, NULL, NULL);
    afficher_texte(renderer, font, buffer, 300, 250, noir); // Texte en noir au centre
    
    SDL_RenderPresent(renderer);
    SDL_Delay(1500); // Pause de 1,5 secondes avant de revenir au jeu

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

// Nouvelle fonction pour afficher l'écran titre
void afficher_ecran_titre(int *running) {
    SDL_Event event;
    int in_title_screen = 1;

    // Charger la texture de l'écran titre
    SDL_Texture *title_texture = load_texture("assets/images/title.jpg");
    if (!title_texture) {
        printf("Erreur de chargement de l'image title.jpg\n");
        *running = 0;
        return;
    }

    // Charger la police
    TTF_Font *font = TTF_OpenFont("assets/fonts/eurostile.ttf", 48);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        *running = 0;
        return;
    }
    SDL_Color blanc = {255, 255, 255};

    while (in_title_screen) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                in_title_screen = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                in_title_screen = 0;
            }
        }

        // Rendu de l'écran titre
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, title_texture, NULL, NULL);

        // Afficher le texte "Cliquez pour commencer !"
        afficher_texte(renderer, font, "Cliquez pour commencer !", 200, 500, blanc);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(title_texture);
    TTF_CloseFont(font);
}

void afficher_evenement_sdl(Groupe *groupe, const char *image_path, const char *texte, 
                            int effet_choix1_nourriture, int effet_choix1_eau, 
                            int effet_choix2_nourriture, int effet_choix2_eau) {
    SDL_Event event;
    int in_event = 1;
    int afficher_resultat = 0; // Indique si on affiche le résultat du choix
    char message_resultat[50] = ""; 

    // Charger l'image de l'événement
    SDL_Texture *event_background = load_texture(image_path);
    if (!event_background) {
        printf("Erreur de chargement de l'image d'événement\n");
        return;
    }

    // Charger les boutons
    SDL_Texture *btn_choix1_texture = load_texture("assets/images/Accept.png");
    SDL_Texture *btn_choix2_texture = load_texture("assets/images/Refuser.png");

    if (!btn_choix1_texture || !btn_choix2_texture) {
        printf("Erreur de chargement des boutons d'événement\n");
        return;
    }

    // Charger les polices
    TTF_Font *font_texte = TTF_OpenFont("assets/fonts/eurostile.ttf", 16); // Texte plus petit
    TTF_Font *font_resultat = TTF_OpenFont("assets/fonts/eurostile.ttf", 32); // Résultat plus gros

    if (!font_texte || !font_resultat) {
        printf("Erreur de chargement des polices\n");
        return;
    }

    SDL_Color blanc = {255, 255, 255};

    // Définition des zones
    SDL_Rect rect_zone_texte = {0, 450, 800, 150}; // Zone noire pour le texte
    SDL_Rect rect_btn_choix1 = {150, 520, 200, 50}; // Bouton "Accepter"
    SDL_Rect rect_btn_choix2 = {450, 520, 200, 50}; // Bouton "Refuser"

    while (in_event) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                in_event = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && !afficher_resultat) {
                int x = event.button.x;
                int y = event.button.y;

                // Si le joueur clique sur le bouton "Accepter"
                if (x >= rect_btn_choix1.x && x <= rect_btn_choix1.x + rect_btn_choix1.w &&
                    y >= rect_btn_choix1.y && y <= rect_btn_choix1.y + rect_btn_choix1.h) {
                    groupe->nourriture += effet_choix1_nourriture;
                    groupe->eau += effet_choix1_eau;
                    snprintf(message_resultat, sizeof(message_resultat), 
                             "+%d Nourriture, +%d Eau", effet_choix1_nourriture, effet_choix1_eau);
                    afficher_resultat = 1;
                }

                // Si le joueur clique sur le bouton "Refuser"
                if (x >= rect_btn_choix2.x && x <= rect_btn_choix2.x + rect_btn_choix2.w &&
                    y >= rect_btn_choix2.y && y <= rect_btn_choix2.y + rect_btn_choix2.h) {
                    groupe->nourriture += effet_choix2_nourriture;
                    groupe->eau += effet_choix2_eau;
                    snprintf(message_resultat, sizeof(message_resultat), 
                             "+%d Nourriture, +%d Eau", effet_choix2_nourriture, effet_choix2_eau);
                    afficher_resultat = 1;
                }
            }
        }

        // Affichage
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, event_background, NULL, NULL);

        // Zone noire pour le texte
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &rect_zone_texte);

        // Affichage de l'énoncé
        afficher_texte(renderer, font_texte, texte, 20, 470, blanc);

        if (!afficher_resultat) {
            // Afficher les boutons si le choix n'est pas encore fait
            SDL_RenderCopy(renderer, btn_choix1_texture, NULL, &rect_btn_choix1);
            SDL_RenderCopy(renderer, btn_choix2_texture, NULL, &rect_btn_choix2);
        } else {
            // Afficher le résultat en grand si un choix a été fait
            afficher_texte(renderer, font_resultat, message_resultat, 300, 550, blanc);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000); // Pause d'1 seconde pour afficher le résultat
            in_event = 0; // Fermer l'événement après affichage du résultat
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyage des textures
    SDL_DestroyTexture(event_background);
    SDL_DestroyTexture(btn_choix1_texture);
    SDL_DestroyTexture(btn_choix2_texture);
    TTF_CloseFont(font_texte);
    TTF_CloseFont(font_resultat);
}
