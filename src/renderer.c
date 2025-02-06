#include "../include/renderer.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include "../include/ui.h"
#include "../include/game.h"
#include <SDL2/SDL_syswm.h>
#include <dirent.h>
#include <sys/stat.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int fullscreen = 0;
int music_on = 1;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *background_texture = NULL;
SDL_Texture *btn_new_game_texture = NULL;
SDL_Texture *btn_options_texture = NULL;
SDL_Texture *btn_quit_texture = NULL;
TTF_Font *font = NULL;

SDL_Texture *icon_walk = NULL;
SDL_Texture *icon_water = NULL;
SDL_Texture *icon_food = NULL;
SDL_Texture *icon_rest = NULL;
SDL_Texture *icon_hunt = NULL;
SDL_Texture *icon_fetch_water = NULL;
SDL_Texture *icon_save = NULL;

SDL_Rect btn_new_game, btn_options, btn_quit;

SDL_Color vert = {0, 255, 0, 255};
SDL_Color rouge = {255, 0, 0, 255};
SDL_Color jaune = {255, 255, 0, 255};
SDL_Color bleu = {0, 0, 255, 255};

SDL_Texture* load_texture(const char *file_path) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, file_path);
    if (!texture) {
        printf("Erreur IMG_LoadTexture : %s\n", IMG_GetError());
    }
    return texture;
}

Mix_Music *musicList[4];
int currentMusicIndex = 0;

void load_music() {
    musicList[0] = Mix_LoadMUS("assets/music/1.wav");
    musicList[1] = Mix_LoadMUS("assets/music/2.wav");
    musicList[2] = Mix_LoadMUS("assets/music/3.wav");
    musicList[3] = Mix_LoadMUS("assets/music/4.wav");
}

void play_music() {
    if (musicList[currentMusicIndex] && Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(musicList[currentMusicIndex], -1);
    }
}

void stop_music() {
    if (Mix_PlayingMusic() != 0) {
        Mix_HaltMusic();
    }
}

int init_renderer() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
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
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur Mix_OpenAudio : %s\n", Mix_GetError());
        return 1;
    }

    load_music();
    if (music_on) {
        play_music();
    }

    window = SDL_CreateWindow("Pioneer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer : %s\n", SDL_GetError());
        return 1;
    }

    background_texture = load_texture("assets/images/menu.jpg");
    btn_new_game_texture = load_texture("assets/images/new.png");
    btn_options_texture = load_texture("assets/images/option.png");
    btn_quit_texture = load_texture("assets/images/quit.png");

    if (!background_texture || !btn_new_game_texture || !btn_options_texture || !btn_quit_texture) {
        return 1;
    }

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

    font = TTF_OpenFont("assets/fonts/eurostile.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return 1;
    }

    return 0;
}

void afficher_menu(int *running, int *start_game, Groupe *groupe, int *loaded_game) {
    SDL_Event event;
    int in_menu = 1;

    SDL_Texture *menu_background = load_texture("assets/images/menu.jpg");
    if (!menu_background) {
        printf("Erreur de chargement du background du menu\n");
        return;
    }

    SDL_Texture *btn_load_texture = load_texture("assets/icons/load.jpg");
    if (!btn_load_texture) {
        printf("Erreur de chargement de l'icône de chargement\n");
        return;
    }

    SDL_Rect btn_load = {WINDOW_WIDTH / 2 - 25, WINDOW_HEIGHT - 60, 50, 50};

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
                    *start_game = 1;
                    init_groupe(groupe);
                    in_menu = 0;
                } else if (x >= btn_options.x && x <= btn_options.x + btn_options.w &&
                           y >= btn_options.y && y <= btn_options.y + btn_options.h) {
                    afficher_options_menu(running);
                } else if (x >= btn_quit.x && x <= btn_quit.x + btn_quit.w &&
                           y >= btn_quit.y && y <= btn_quit.y + btn_quit.h) {
                    *running = 0;
                    in_menu = 0;
                } else if (x >= btn_load.x && x <= btn_load.x + btn_load.w &&
                           y >= btn_load.y && y <= btn_load.y + btn_load.h) {
                    
                    char filepath[256] = {0};
                    if (sdl_file_dialog(filepath, sizeof(filepath))) {
                        printf("Chargement du jeu depuis %s\n", filepath); 
                        if (load_game(filepath, groupe)) {
                            printf("Jeu chargé avec succès\n");
                            *start_game = 1;
                            *loaded_game = 1;
                            in_menu = 0;
                        } else {
                            printf("Erreur lors du chargement du fichier de sauvegarde\n");
                        }
                    } else {
                        printf("Aucun fichier sélectionné\n");
                    }
                }
            }
        }

        // Rendu du menu
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, menu_background, NULL, NULL);
        SDL_RenderCopy(renderer, btn_new_game_texture, NULL, &btn_new_game);
        SDL_RenderCopy(renderer, btn_options_texture, NULL, &btn_options);
        SDL_RenderCopy(renderer, btn_quit_texture, NULL, &btn_quit);
        SDL_RenderCopy(renderer, btn_load_texture, NULL, &btn_load);
        SDL_RenderPresent(renderer);
    }

    // Libération des textures après utilisation
    SDL_DestroyTexture(menu_background);
    SDL_DestroyTexture(btn_load_texture);
}

int sdl_file_dialog(char *filepath, size_t filepath_size) {
    SDL_Event event;
    int dialog_open = 1;
    int file_selected = 0;

    // Lister les fichiers dans le répertoire "saves"
    struct dirent *entry;
    DIR *dp = opendir("saves");
    if (dp == NULL) {
        printf("Erreur : Impossible d'ouvrir le dossier des sauvegardes\n");
        return 0;
    }

    const char *files[256];
    int file_count = 0;
    while ((entry = readdir(dp)) != NULL) {
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "saves/%s", entry->d_name);
        struct stat path_stat;
        stat(full_path, &path_stat);
        if (S_ISREG(path_stat.st_mode) && strstr(entry->d_name, ".txt")) {
            files[file_count++] = strdup(entry->d_name);
        }
    }
    closedir(dp);

    SDL_Rect dialog_rect = {200, 150, 400, 300};
    SDL_Rect file_rects[file_count];
    for (int i = 0; i < file_count; i++) {
        file_rects[i] = (SDL_Rect){dialog_rect.x + 20, dialog_rect.y + 50 + i * 40, 360, 30};
    }

    while (dialog_open) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                dialog_open = 0;
                return 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                for (int i = 0; i < file_count; i++) {
                    if (x >= file_rects[i].x && x <= file_rects[i].x + file_rects[i].w &&
                        y >= file_rects[i].y && y <= file_rects[i].y + file_rects[i].h) {
                        snprintf(filepath, filepath_size, "saves/%s", files[i]);
                        file_selected = 1;
                        dialog_open = 0;
                        break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &dialog_rect);

        for (int i = 0; i < file_count; i++) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &file_rects[i]);
            afficher_texte(renderer, font, files[i], file_rects[i].x + 10, file_rects[i].y + 5, (SDL_Color){0, 0, 0, 255});
        }

        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < file_count; i++) {
        free((void *)files[i]);
    }

    return file_selected;
}

SDL_Color blanc = {255, 255, 255};

void afficher_options_menu(int *running) {
    SDL_Event event;
    int in_options_menu = 1;

    while (in_options_menu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                in_options_menu = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_f:
                        fullscreen = !fullscreen;
                        toggle_fullscreen();
                        break;
                    case SDLK_m:
                        music_on = !music_on;
                        toggle_music();
                        break;
                    case SDLK_ESCAPE:
                        in_options_menu = 0;
                        break;
                    default:
                        break;
                }
            }
        }

        // Remplir l'arrière-plan
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Afficher des instructions simples
        afficher_texte(renderer, font, "Appuyez sur F pour basculer en plein écran", 100, 100, blanc);
        afficher_texte(renderer, font, fullscreen ? "Actuellement : PLEIN ÉCRAN ACTIVÉ"
                                                  : "Actuellement : PLEIN ÉCRAN DÉSACTIVÉ",
                       100, 150, blanc);

        afficher_texte(renderer, font, "Appuyez sur M pour activer/désactiver la musique", 100, 200, blanc);
        afficher_texte(renderer, font, music_on ? "Actuellement : MUSIQUE ACTIVÉE"
                                                : "Actuellement : MUSIQUE DÉSACTIVÉE",
                       100, 250, blanc);

        afficher_texte(renderer, font, "Appuyez sur ESC pour revenir", 100, 300, blanc);

        SDL_RenderPresent(renderer);
    }
}

void toggle_fullscreen() {
    if (fullscreen) {
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    } else {
        SDL_SetWindowFullscreen(window, 0);
    }
}

void toggle_music() {
    if (music_on) {
        play_music();
    } else {
        stop_music();
    }
}

void afficher_jeu(int *running, Groupe *groupe, int *return_to_menu) {
    SDL_Event event;
    int in_game = 1;
    Uint32 save_message_time = 0;

    ActionsJour actions;
    init_actions_jour(&actions);

    TTF_Font *font = TTF_OpenFont("assets/fonts/eurostile.ttf", 20);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return;
    }
    SDL_Color blanc = {255, 255, 255};
    SDL_Color noir = {0, 0, 0};

    background_texture = load_texture("assets/images/game_background.jpg");
    icon_walk = load_texture("assets/icons/walk.png");
    icon_water = load_texture("assets/icons/drink.png");
    icon_food = load_texture("assets/icons/eat.png");
    icon_rest = load_texture("assets/icons/rest.png");
    icon_hunt = load_texture("assets/icons/hunt.png");
    icon_fetch_water = load_texture("assets/icons/fetch_water.png");
    icon_save = load_texture("assets/icons/save.jpg");

    SDL_Rect rect_background = {0, 100, 800, 400};
    SDL_Rect rect_infos = {0, 0, 800, 120};
    SDL_Rect rect_actions = {0, 500, 800, 100};
    SDL_Rect rect_fin_jour = {600, 520, 150, 50};
    SDL_Rect rect_save = {10, 10, 40, 40};
    SDL_Rect rect_exit = {600, 560, 150, 50};

    int icon_size = 50;
    int icon_border = 4;

    SDL_Rect rect_walk = {30, 520, icon_size, icon_size};
    SDL_Rect rect_water = {130, 520, icon_size, icon_size};
    SDL_Rect rect_food = {230, 520, icon_size, icon_size};
    SDL_Rect rect_rest = {330, 520, icon_size, icon_size};
    SDL_Rect rect_hunt = {430, 520, icon_size, icon_size};
    SDL_Rect rect_fetch_water = {530, 520, icon_size, icon_size};

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
                } else if (x >= rect_save.x && x <= rect_save.x + rect_save.w && y >= rect_save.y && y <= rect_save.y + rect_save.h) {
                    save_game(groupe);
                    save_message_time = SDL_GetTicks();
                } else if (x >= rect_exit.x && x <= rect_exit.x + rect_exit.w && y >= rect_exit.y && y <= rect_exit.y + rect_exit.h) {
                    if (confirm_exit()) {
                        *running = 0;
                        in_game = 0;
                    }
                }

                if (x >= rect_fin_jour.x && x <= rect_fin_jour.x + rect_fin_jour.w &&
                    y >= rect_fin_jour.y && y <= rect_fin_jour.y + rect_fin_jour.h) {
                    afficher_transition_jour(renderer, groupe->joursPasses + 1);
                    appliquer_actions(groupe, &actions);
                    selected_actions = 0;

                    if (groupe->sante <= 0) {
                        afficher_ecran_game_over(running, return_to_menu);
                        in_game = 0;
                    }
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background_texture, NULL, &rect_background);

        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderFillRect(renderer, &rect_infos);

        char buffer[256];
        sprintf(buffer, "Jour: %d | Distance: %d km", groupe->joursPasses, groupe->distanceRestante);
        afficher_texte(renderer, font, buffer, 20, 20, blanc);

        int texte_x_gauche = 20;
        int barre_x_gauche = 200;
        int texte_x_droite = 430;
        int barre_x_droite = 570;
        int barre_largeur = 200;
        int barre_hauteur = 12;

        sprintf(buffer, "Colons: %d", groupe->colons);
        afficher_texte(renderer, font, buffer, texte_x_droite, 20, blanc);
        afficher_barre(renderer, barre_x_droite, 30, barre_largeur, barre_hauteur, groupe->colons, 100, (SDL_Color){255, 165, 0, 255});

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

        SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
        SDL_RenderFillRect(renderer, &rect_actions);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if (actions.avancer) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_walk.x - icon_border, rect_walk.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.boire) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_water.x - icon_border, rect_water.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.manger) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_food.x - icon_border, rect_food.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.se_reposer) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_rest.x - icon_border, rect_rest.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.chasser) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_hunt.x - icon_border, rect_hunt.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});
        if (actions.chercher_eau) SDL_RenderFillRect(renderer, &(SDL_Rect){rect_fetch_water.x - icon_border, rect_fetch_water.y - icon_border, icon_size + (icon_border * 2), icon_size + (icon_border * 2)});

        SDL_RenderCopy(renderer, icon_walk, NULL, &rect_walk);
        SDL_RenderCopy(renderer, icon_water, NULL, &rect_water);
        SDL_RenderCopy(renderer, icon_food, NULL, &rect_food);
        SDL_RenderCopy(renderer, icon_rest, NULL, &rect_rest);
        SDL_RenderCopy(renderer, icon_hunt, NULL, &rect_hunt);
        SDL_RenderCopy(renderer, icon_fetch_water, NULL, &rect_fetch_water);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &rect_fin_jour);
        afficher_texte(renderer, font, "Fin du jour", rect_fin_jour.x + 10, rect_fin_jour.y + 10, blanc);

        SDL_RenderCopy(renderer, icon_save, NULL, &rect_save);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &rect_exit);
        afficher_texte(renderer, font, "Quitter le jeu", rect_exit.x + 10, rect_exit.y + 10, blanc);

        if (save_message_time > 0 && SDL_GetTicks() - save_message_time < 2000) {
            afficher_texte(renderer, font, "Partie sauvegardée", 300, 250, noir);
        } else {
            save_message_time = 0;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(icon_walk);
    SDL_DestroyTexture(icon_water);
    SDL_DestroyTexture(icon_food);
    SDL_DestroyTexture(icon_rest);
    SDL_DestroyTexture(icon_hunt);
    SDL_DestroyTexture(icon_fetch_water);
    SDL_DestroyTexture(icon_save);
    TTF_CloseFont(font);
}

int confirm_exit() {
    SDL_Event event;
    int dialog_open = 1;
    int confirm = 0;

    SDL_Rect dialog_rect = {200, 150, 400, 300};

    SDL_Rect btn_yes = {250, 350, 100, 50};
    SDL_Rect btn_no = {450, 350, 100, 50};

    TTF_Font *font = TTF_OpenFont("assets/fonts/eurostile.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return 0;
    }
    SDL_Color blanc = {255, 255, 255};

    while (dialog_open) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                dialog_open = 0;
                return 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                if (x >= btn_yes.x && x <= btn_yes.x + btn_yes.w &&
                    y >= btn_yes.y && y <= btn_yes.y + btn_yes.h) {
                    confirm = 1;
                    dialog_open = 0;
                } else if (x >= btn_no.x && x <= btn_no.x + btn_no.w &&
                           y >= btn_no.y && y <= btn_no.y + btn_no.h) {
                    dialog_open = 0;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &dialog_rect);

        afficher_texte(renderer, font, "Etes-vous sûr de quitter le jeu ?", 220, 200, blanc);
        afficher_texte(renderer, font, "Toute progression non sauvegardée sera perdue", 220, 250, blanc);

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &btn_yes);
        SDL_RenderFillRect(renderer, &btn_no);
        afficher_texte(renderer, font, "Oui", btn_yes.x + 30, btn_yes.y + 10, blanc);
        afficher_texte(renderer, font, "Non", btn_no.x + 30, btn_no.y + 10, blanc);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    return confirm;
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
    SDL_Color noir = {0, 0, 0, 255};
    TTF_Font *font = TTF_OpenFont("assets/fonts/Opensans.ttf", 48);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *transition_texture = load_texture("assets/images/transition.jpg");
    if (!transition_texture) {
        printf("Erreur : Impossible de charger l'image de transition\n");
        return;
    }

    char buffer[32];
    sprintf(buffer, "Jour %d", jour);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, transition_texture, NULL, NULL);
    afficher_texte(renderer, font, buffer, 300, 250, noir);
    
    SDL_RenderPresent(renderer);
    SDL_Delay(1500);

    SDL_DestroyTexture(transition_texture);
    TTF_CloseFont(font);
}

void afficher_barre(SDL_Renderer *renderer, int x, int y, int largeur, int hauteur, int valeur, int max, SDL_Color couleur) {
    int barre_largeur = (valeur * largeur) / max;

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect fond = {x, y, largeur, hauteur};
    SDL_RenderFillRect(renderer, &fond);

    SDL_SetRenderDrawColor(renderer, couleur.r, couleur.g, couleur.b, 255);
    SDL_Rect barre = {x, y, barre_largeur, hauteur};
    SDL_RenderFillRect(renderer, &barre);
}

void afficher_ecran_titre(int *running) {
    SDL_Event event;
    int in_title_screen = 1;

    SDL_Texture *title_texture = load_texture("assets/images/title.jpg");
    if (!title_texture) {
        printf("Erreur de chargement de l'image title.jpg\n");
        *running = 0;
        return;
    }

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

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, title_texture, NULL, NULL);

        afficher_texte(renderer, font, "Cliquez pour commencer !", 200, 500, blanc);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(title_texture);
    TTF_CloseFont(font);
}

void afficher_evenement_sdl(
    Groupe *groupe, 
    const char *image_path, 
    const char *texte,
    int effet_choix1_nourriture, 
    int effet_choix1_eau,
    int effet_choix2_nourriture, 
    int effet_choix2_eau,
    int effet_choix1_colons,
    int effet_choix2_colons,
    int effet_choix1_sante,
    int effet_choix2_sante,
    int effet_choix1_fatigue,
    int effet_choix2_fatigue,
    int effet_choix1_distance,
    int effet_choix2_distance
) {
    SDL_Event event;
    int in_event = 1;
    int afficher_resultat = 0;
    char message_resultat[100] = ""; 

    SDL_Texture *event_background = load_texture(image_path);
    if (!event_background) {
        printf("Erreur de chargement de l'image d'événement\n");
        return;
    }

    SDL_Texture *btn_choix1_texture = load_texture("assets/images/Accept.png");
    SDL_Texture *btn_choix2_texture = load_texture("assets/images/Refuser.png");

    if (!btn_choix1_texture || !btn_choix2_texture) {
        printf("Erreur de chargement des boutons d'événement\n");
        return;
    }

    TTF_Font *font_texte = TTF_OpenFont("assets/fonts/eurostile.ttf", 16);
    TTF_Font *font_resultat = TTF_OpenFont("assets/fonts/eurostile.ttf", 32);

    if (!font_texte || !font_resultat) {
        printf("Erreur de chargement des polices\n");
        return;
    }

    SDL_Color blanc = {255, 255, 255};

    SDL_Rect rect_zone_texte = {0, 450, 800, 150};
    SDL_Rect rect_btn_choix1 = {150, 520, 200, 50};
    SDL_Rect rect_btn_choix2 = {450, 520, 200, 50};

    while (in_event) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                in_event = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && !afficher_resultat) {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= rect_btn_choix1.x && x <= rect_btn_choix1.x + rect_btn_choix1.w &&
                    y >= rect_btn_choix1.y && y <= rect_btn_choix1.y + rect_btn_choix1.h) {
                    groupe->nourriture += effet_choix1_nourriture;
                    groupe->eau += effet_choix1_eau;
                    groupe->colons += effet_choix1_colons;
                    groupe->sante += effet_choix1_sante;
                    groupe->fatigue += effet_choix1_fatigue;
                    groupe->distanceRestante += effet_choix1_distance;
                    snprintf(message_resultat, sizeof(message_resultat), 
                             "Choix 1: %+d Nourriture, %+d Eau, %+d Colons, %+d Santé, %+d Fatigue, %+d Distance",
                             effet_choix1_nourriture, effet_choix1_eau, effet_choix1_colons, effet_choix1_sante, effet_choix1_fatigue, effet_choix1_distance);
                    afficher_resultat = 1;
                }

                if (x >= rect_btn_choix2.x && x <= rect_btn_choix2.x + rect_btn_choix2.w &&
                    y >= rect_btn_choix2.y && y <= rect_btn_choix2.y + rect_btn_choix2.h) {
                    groupe->nourriture += effet_choix2_nourriture;
                    groupe->eau += effet_choix2_eau;
                    groupe->colons += effet_choix2_colons;
                    groupe->sante += effet_choix2_sante;
                    groupe->fatigue += effet_choix2_fatigue;
                    groupe->distanceRestante += effet_choix2_distance;
                    snprintf(message_resultat, sizeof(message_resultat), 
                             "Choix 2: %+d Nourriture, %+d Eau, %+d Colons, %+d Santé, %+d Fatigue, %+d Distance",
                             effet_choix2_nourriture, effet_choix2_eau, effet_choix2_colons, effet_choix2_sante, effet_choix2_fatigue, effet_choix2_distance);
                    afficher_resultat = 1;
                }
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, event_background, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
        SDL_RenderFillRect(renderer, &rect_zone_texte);

        afficher_texte(renderer, font_texte, texte, 20, 470, blanc);

        if (!afficher_resultat) {
            SDL_RenderCopy(renderer, btn_choix1_texture, NULL, &rect_btn_choix1);
            SDL_RenderCopy(renderer, btn_choix2_texture, NULL, &rect_btn_choix2);
        } else {
            afficher_texte(renderer, font_resultat, message_resultat, 300, 550, blanc);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            in_event = 0;
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(event_background);
    SDL_DestroyTexture(btn_choix1_texture);
    SDL_DestroyTexture(btn_choix2_texture);
    TTF_CloseFont(font_texte);
    TTF_CloseFont(font_resultat);
}

void afficher_ecran_game_over(int *running, int *return_to_menu) {
    SDL_Event event;
    int in_game_over = 1;

    printf("Attempting to load game over image from: assets/images/game_over.jpg\n");

    SDL_Texture *game_over_texture = load_texture("assets/images/game_over.jpg");
    if (!game_over_texture) {
        printf("Erreur de chargement de l'image game_over.jpg\n");
        *running = 0;
        return;
    }

    TTF_Font *font = TTF_OpenFont("assets/fonts/eurostile.ttf", 48);
    if (!font) {
        printf("Erreur TTF_OpenFont : %s\n", TTF_GetError());
        *running = 0;
        return;
    }
    SDL_Color blanc = {255, 255, 255};

    while (in_game_over) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *running = 0;
                in_game_over = 0;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                in_game_over = 0;
                *return_to_menu = 1;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, game_over_texture, NULL, NULL);

        afficher_texte(renderer, font, "Game Over", 300, 250, blanc);

        afficher_texte(renderer, font, "Cliquez pour retourner au menu", 150, 500, blanc);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(game_over_texture);
    TTF_CloseFont(font);
}
