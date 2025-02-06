#include "../include/game.h"
#include "../include/renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Charger le jeu depuis un fichier de sauvegarde
int load_game(const char *filepath, Groupe *groupe) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filepath);
        return 0;
    }

    printf("Lecture sauvefarde...\n"); 

    if (fscanf(file, "Jour: %d\n", &groupe->joursPasses) != 1) {
        printf("Erreur de lecture du jour\n");
        fclose(file);
        return 0;
    }
    if (fscanf(file, "Colons: %d\n", &groupe->colons) != 1) {
        printf("Erreur de lecture des colons\n");
        fclose(file);
        return 0;
    }
    if (fscanf(file, "Santé: %d\n", &groupe->sante) != 1) {
        printf("Erreur de lecture de la santé\n");
        fclose(file);
        return 0;
    }
    if (fscanf(file, "Nourriture: %d\n", &groupe->nourriture) != 1) {
        printf("Erreur de lecture de la nourriture\n");
        fclose(file);
        return 0;
    }
    if (fscanf(file, "Eau: %d\n", &groupe->eau) != 1) {
        printf("Erreur de lecture de l'eau\n");
        fclose(file);
        return 0;
    }
    if (fscanf(file, "Fatigue: %d\n", &groupe->fatigue) != 1) {
        printf("Erreur de lecture de la fatigue\n");
        fclose(file);
        return 0;
    }
    if (fscanf(file, "Distance restante: %d\n", &groupe->distanceRestante) != 1) {
        printf("Erreur de lecture de la distance restante\n");
        fclose(file);
        return 0;
    }

    printf("Sauvegarde lue\n"); 
    printf("Jour: %d, Colons: %d, Santé: %d, Nourriture: %d, Eau: %d, Fatigue: %d, Distance restante: %d\n",
           groupe->joursPasses, groupe->colons, groupe->sante, groupe->nourriture, groupe->eau, groupe->fatigue, groupe->distanceRestante);

    fclose(file);
    return 1;
}

// Initialisation du groupe
void init_groupe(Groupe *groupe) {
    groupe->colons = 10;
    groupe->sante = 100;
    groupe->puissanceAttaque = 5;
    groupe->experience = 0;
    groupe->nourriture = 50;
    groupe->eau = 50;
    groupe->fatigue = 0;
    groupe->joursSansEau = 0;
    groupe->joursSansNourriture = 0;
    groupe->joursPasses = 0;
    groupe->distanceRestante = 500;
    groupe->vitesse = 10;
    groupe->temperature = 35;
}

// Initialisation des actions du jour
void init_actions_jour(ActionsJour *actions) {
    actions->boire = 0;
    actions->manger = 0;
    actions->avancer = 0;
    actions->se_reposer = 0;
    actions->chercher_eau = 0;
    actions->chasser = 0;
}

// Appliquer les actions du joueur
void appliquer_actions(Groupe *groupe, ActionsJour *actions) {
    int colons = groupe->colons;

    // Consommation de base
    int base_eau = 4 + (colons > 10 ? (colons - 10) / 3 : 0);
    int base_nourriture = 3 + (colons > 10 ? (colons - 10) / 2 : 0);

    // Si le joueur marche
    if (actions->avancer) {
        groupe->distanceRestante -= 10;
        groupe->eau -= 6 + (colons > 10 ? (colons - 10) / 5 : 0);
        groupe->nourriture -= 5 + (colons > 10 ? (colons - 10) / 5 : 0);
        groupe->fatigue += 7 + (colons > 15 ? (colons - 15) / 5 * 2 : 0);
    }

    // Si le joueur boit
    if (actions->boire && groupe->eau > 0) {
        groupe->eau -= 5 + (colons > 10 ? (colons - 10) / 3 : 0);
        if (groupe->joursSansEau > 0) {
            groupe->sante += 5;
            groupe->joursSansEau = 0;
        }
    }

    // Si le joueur mange
    if (actions->manger && groupe->nourriture > 0) {
        groupe->nourriture -= 5 + (colons > 10 ? (colons - 10) / 2 : 0);
        if (groupe->joursSansNourriture > 0) {
            groupe->sante += 5;
            groupe->joursSansNourriture = 0;
        }
    }

    // Si le joueur se repose
    if (actions->se_reposer) {
        groupe->fatigue -= 15 + (colons > 15 ? 2 : 0) + (colons > 20 ? 5 : 0);
        if (groupe->fatigue < 0) groupe->fatigue = 0;
        groupe->nourriture -= 3 + (colons > 10 ? (colons - 10) / 5 : 0);
        groupe->eau -= 3 + (colons > 10 ? (colons - 10) / 5 : 0);
        groupe->sante += 2;
    }

    // Nouvelle action : Chasser
    if (actions->chasser) {
        int success_rate = 50 + (colons > 10 ? (colons - 10) / 5 * 5 : 0);
        if (rand() % 100 < success_rate) {
            groupe->nourriture += 12 + rand() % 11;
            groupe->fatigue += 10 + rand() % 5;
        } else {
            groupe->sante -= 5 + rand() % 8;
            groupe->fatigue += 10 + rand() % 5;
        }
    }

    // Nouvelle action : Chercher de l'eau
    if (actions->chercher_eau) {
        int success_rate = 50 + (colons > 10 ? (colons - 10) / 5 * 5 : 0);
        if (rand() % 100 < success_rate) {
            groupe->eau += 12 + rand() % 11;
            groupe->fatigue += 6 + rand() % 5;
        } else {
            groupe->fatigue += 10 + rand() % 5;
        }
    }

    // Si aucune action n'est prise
    if (!actions->avancer && !actions->boire && !actions->manger && !actions->se_reposer && !actions->chasser && !actions->chercher_eau) {
        groupe->sante -= 2;  
        groupe->eau -= base_eau;
        groupe->nourriture -= base_nourriture;
        groupe->fatigue += 3;  
    }

    // Conditions de survie 
    if (groupe->eau <= 0) {
        groupe->joursSansEau++;
    } else {
        groupe->joursSansEau = 0;
    }

    if (groupe->nourriture <= 0) {
        groupe->joursSansNourriture++;
    } else {
        groupe->joursSansNourriture = 0;
    }

    if (groupe->joursSansEau >= 3) {
        groupe->sante -= 20;  
    }
    
    if (groupe->joursSansNourriture >= 5) {
        groupe->sante -= 10;  
    }

    if (groupe->fatigue >= 90) {
        groupe->vitesse = 5;  
    } else {
        groupe->vitesse = 10; 
    }

    groupe->joursPasses++;

    if (rand() % 3 == 0) { 
        declencher_evenement(groupe);
    }

    if (groupe->sante <= 0 || groupe->colons <= 0) {
        printf("Perdu ! Votre colonie n'a pas survécu.\n");
    }

    init_actions_jour(actions);
}

extern SDL_Renderer *renderer; 
extern TTF_Font *font;

// Déclencher un événement aléatoire
void declencher_evenement(Groupe *groupe) {
    int choix_evenement = rand() % 30; 
    const char *image_path;

    switch (choix_evenement) {
        case 0:
            image_path = "assets/images/oldman.jpg";
            afficher_evenement_sdl(groupe, image_path, 
                "Un vieil homme propose de partager ses rations en échange d’eau. Acceptez-vous ?", 
                10, -5, // Nourriture, Eau
                0, 0,   // Nourriture, Eau
                0, 0,   // Colons
                0, 0,   // Santé
                0, 0,   // Fatigue
                0, 0    // Distance
            );
            break;

        case 1:
            image_path = "assets/images/sandstorm.jpg";
            afficher_evenement_sdl(groupe, image_path, 
                "Une tempête de sable approche ! On vous propose de vous cacher dans une grotte. Que faites-vous ?", 
                0, -5,   // Fatigue
                -10, -3, // Santé, Eau
                0, 0,    // Colons
                0, -10,  // Santé
                -5, 0,   // Fatigue
                0, 0     // Distance
            );
            break;

        case 2:
            image_path = "assets/images/camp.jpg";
            afficher_evenement_sdl(groupe, image_path,
                "Vous trouvez un camp abandonné. Voulez-vous fouiller ?", 
                15, 10,  // Nourriture, Eau
                0, 0,    // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );
            break;
        
        case 3:  
            image_path = "assets/images/trancho.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Un homme mystérieux au nom de Trancho vous propose de l'aide. Que faire ?",  
                10, 5,  // Nourriture, Eau
                -20, 0, // Santé
                0, 0,   // Colons
                0, -20, // Santé
                0, 0,   // Fatigue
                0, 0    // Distance
            );  
            break;

        case 4:
            image_path = "assets/images/oasis.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Un éclaireur signale une oasis au loin. Vous avez peu d’eau mais il y a un risque d’attaque. Que faites-vous ?",  
                20, 10,  // Eau, Nourriture
                -5, 0,   // Santé
                0, 0,    // Colons
                0, -5,   // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 5:
            image_path = "assets/images/scorpion.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Un colon est piqué par un scorpion ! Utilisez-vous des ressources pour le soigner ?",  
                -5, -5,  // Nourriture, Eau
                -15, 0,  // Santé
                0, 0,    // Colons
                0, -15,  // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 6:
            image_path = "assets/images/nomads.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Un groupe de nomades marchands traverse votre chemin. Ils vous offrent un marché. Que faites-vous ?",  
                -5, 15,  // Nourriture, Eau
                5, -5,   // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 7:
            image_path = "assets/images/ruins.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Vous découvrez des ruines antiques. L’exploration pourrait être dangereuse. Voulez-vous fouiller ?",  
                25, 5,   // Nourriture, Eau
                -10, 0,  // Santé
                0, 0,    // Colons
                0, -10,  // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 8:
            image_path = "assets/images/night_attack.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Des pillards attaquent votre camp pendant la nuit. Vous devez vous défendre !",  
                -10, -10,  // Nourriture, Eau
                -20, -20,  // Santé
                0, 0,      // Colons
                -10, -20,  // Santé
                0, 0,      // Fatigue
                0, 0       // Distance
            );  
            break;

        case 9:
            image_path = "assets/images/mirage.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Vous apercevez un mirage au loin. Voulez-vous le suivre ?",  
                0, -10,  // Eau
                0, 0,    // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 10:
            image_path = "assets/images/merchant.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Un marchand ambulant vous propose des vivres en échange de vos armes. Acceptez-vous ?",  
                20, -10,  // Nourriture, Eau
                0, 0,     // Nourriture, Eau
                0, 0,     // Colons
                -10, 0,   // Santé
                0, 0,     // Fatigue
                0, 0      // Distance
            );  
            break;

        case 11:
            image_path = "assets/images/abandoned_well.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Vous trouvez un puits abandonné. Voulez-vous tenter de puiser de l'eau ?",  
                0, 20,   // Eau
                0, 0,    // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 12:
            image_path = "assets/images/stray_dog.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Un chien errant vous suit. Voulez-vous le nourrir ?",  
                -5, 0,   // Nourriture
                0, 0,    // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 13:
            image_path = "assets/images/abandoned_village.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Vous découvrez un village abandonné. Voulez-vous y chercher des survivants ?",  
                0, 0,    // Nourriture, Eau
                0, 0,    // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 14:
            image_path = "assets/images/stranger.jpg";
            afficher_evenement_sdl(groupe, image_path,
                "Un étranger vous propose de rejoindre votre groupe. Acceptez-vous ?",
                0, 0,   // Nourriture, Eau
                0, 0,   // Nourriture, Eau
                1, 0,   // Colons
                0, 0,   // Santé
                0, 0,   // Fatigue
                0, 0    // Distance
            );
            break;

        case 15:
            image_path = "assets/images/trancho2.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Trancho, un homme mystérieux et drôle, vous propose de l'aide en échange d'une blague. Acceptez-vous ?",  
                10, 5,  // Nourriture, Eau
                -5, 0,  // Santé
                0, 0,   // Colons
                0, -5,  // Santé
                0, 0,   // Fatigue
                0, 0    // Distance
            );  
            break;

        case 16:
            image_path = "assets/images/trancho2.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Trancho vous propose un défi : résoudre une énigme pour gagner des ressources. Acceptez-vous ?",  
                15, 10,  // Nourriture, Eau
                -10, 0,  // Santé
                0, 0,    // Colons
                0, -10,  // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 17:
            image_path = "assets/images/trancho.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Trancho vous raconte une histoire hilarante. Vous riez tellement que vous en oubliez de boire. Que faites-vous ?",  
                0, -10,  // Eau
                0, 0,    // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 18:
            image_path = "assets/images/lost_colon.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Vous trouvez un colon perdu dans le désert. Voulez-vous l'accueillir ?",  
                0, 0,    // Nourriture, Eau
                0, 0,    // Nourriture, Eau
                1, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                0, 0     // Distance
            );  
            break;

        case 19:
            image_path = "assets/images/sick_colon.jpg";
            afficher_evenement_sdl(groupe, image_path,
                "Un de vos colons tombe gravement malade. Utilisez-vous des ressources pour le soigner ?",
                -5, -5, // Nourriture, Eau
                0, 0,   // Nourriture, Eau
                0, -1,  // Colons
                0, 0,   // Santé
                0, 0,   // Fatigue
                0, 0    // Distance
            );
            break;

        case 20:
            image_path = "assets/images/colon_birth.jpg";
            afficher_evenement_sdl(groupe, image_path,
                "Une naissance a lieu dans votre groupe. Voulez-vous célébrer ?",
                -10, -10, // Nourriture, Eau
                0, 0,     // Nourriture, Eau
                1, 0,     // Colons
                0, 0,     // Santé
                0, 0,     // Fatigue
                0, 0      // Distance
            );
            break;

        case 21:
            image_path = "assets/images/shortcut.jpg";
            afficher_evenement_sdl(groupe, image_path,  
                "Vous trouvez un raccourci à travers le désert. Voulez-vous l'emprunter ?",  
                0, 0,    // Nourriture, Eau
                0, 0,    // Nourriture, Eau
                0, 0,    // Colons
                0, 0,    // Santé
                0, 0,    // Fatigue
                -50, 0   // Distance
            );  
            groupe->distanceRestante -= 50;
            break;

        case 27:
            image_path = "assets/images/bandits.jpg";
            afficher_evenement_sdl(groupe, image_path, 
                "Des bandits attaquent votre camp ! Préparez-vous à vous défendre.", 
                0, 0,  // Nourriture, Eau
                0, 0,  // Nourriture, Eau
                0, 0,  // Colons
                0, 0,  // Santé
                0, 0,  // Fatigue
                0, 0   // Distance
            );
            Personnage joueur = creer_personnage("Joueur", creer_stats(groupe->sante, 50, 25, 100));
            Personnage adversaire = creer_personnage("Bandit", creer_stats(100, 50, 25, 100));
            combat_sdl(renderer, font, &joueur, &adversaire);
            groupe->sante = joueur.actuel.vie;
            break;

        case 28:
            image_path = "assets/images/wild_animals.jpg";
            afficher_evenement_sdl(groupe, image_path, 
                "Des animaux sauvages attaquent ! Préparez-vous à vous défendre.", 
                0, 0,  // Nourriture, Eau
                0, 0,  // Nourriture, Eau
                0, 0,  // Colons
                0, 0,  // Santé
                0, 0,  // Fatigue
                0, 0   // Distance
            );
            Personnage joueur2 = creer_personnage("Joueur", creer_stats(groupe->sante, 50, 25, 100));
            Personnage adversaire2 = creer_personnage("Animal Sauvage", creer_stats(100, 50, 25, 100));
            combat_sdl(renderer, font, &joueur2, &adversaire2);
            groupe->sante = joueur2.actuel.vie;
            break;

        case 29:
            image_path = "assets/images/rival_group.jpg";
            afficher_evenement_sdl(groupe, image_path, 
                "Un groupe rival vous attaque ! Préparez-vous à vous défendre.", 
                0, 0,  // Nourriture, Eau
                0, 0,  // Nourriture, Eau
                0, 0,  // Colons
                0, 0,  // Santé
                0, 0,  // Fatigue
                0, 0   // Distance
            );
            Personnage joueur3 = creer_personnage("Joueur", creer_stats(groupe->sante, 50, 25, 100));
            Personnage adversaire3 = creer_personnage("Rival", creer_stats(100, 50, 25, 100));
            combat_sdl(renderer, font, &joueur3, &adversaire3);
            groupe->sante = joueur3.actuel.vie;
            break;
    }
}

// Sauvegarder le jeu dans un fichier
void save_game(Groupe *groupe) {
    char filename[256];
    sprintf(filename, "saves/day_%d.txt", groupe->joursPasses);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erreur lors de la création du fichier de sauvegarde\n");
        return;
    }

    fprintf(file, "Jour: %d\n", groupe->joursPasses);
    fprintf(file, "Colons: %d\n", groupe->colons);
    fprintf(file, "Santé: %d\n", groupe->sante);
    fprintf(file, "Nourriture: %d\n", groupe->nourriture);
    fprintf(file, "Eau: %d\n", groupe->eau);
    fprintf(file, "Fatigue: %d\n", groupe->fatigue);
    fprintf(file, "Distance restante: %d\n", groupe->distanceRestante);

    fclose(file);
    printf("Jeu sauvegardé dans %s\n", filename);
}

// Gérer un combat SDL
void combat_sdl(SDL_Renderer *renderer, TTF_Font *font, Personnage *joueur, Personnage *adversaire) {
    SDL_Event event;
    int in_combat = 1;

    while (in_combat && est_vivant(joueur) && est_vivant(adversaire)) {
        SDL_RenderClear(renderer);

        afficher_personnages_sdl(renderer, font, joueur, adversaire);
        afficher_sorts_sdl(renderer, font);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                in_combat = 0;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                SortId joueur_sort = SORT_AUCUNE;
                if (x >= 100 && x <= 300) {
                    if (y >= 350 && y <= 380) joueur_sort = SORT_COUP;
                    else if (y >= 400 && y <= 430) joueur_sort = SORT_REDUIT_ATTAQUE;
                    else if (y >= 450 && y <= 480) joueur_sort = SORT_REDUIT_DEFENSE;
                    else if (y >= 500 && y <= 530) joueur_sort = SORT_REDUIT_VITESSE;
                } else if (x >= 400 && x <= 600) {
                    if (y >= 350 && y <= 380) joueur_sort = SORT_SOIN;
                    else if (y >= 400 && y <= 430) joueur_sort = SORT_AUGMENTE_ATTAQUE;
                    else if (y >= 450 && y <= 480) joueur_sort = SORT_AUGMENTE_DEFENSE;
                    else if (y >= 500 && y <= 530) joueur_sort = SORT_AUGMENTE_VITESSE;
                }

                if (joueur_sort != SORT_AUCUNE) {
                    SortId adversaire_sort = choisir_action_adversaire(adversaire, joueur);

                    if (joueur->actuel.vitesse >= adversaire->actuel.vitesse) {
                        appliquer_sort(joueur_sort, joueur, adversaire);
                        if (est_vivant(adversaire)) {
                            appliquer_sort(adversaire_sort, adversaire, joueur);
                        }
                    } else {
                        appliquer_sort(adversaire_sort, adversaire, joueur);
                        if (est_vivant(joueur)) {
                            appliquer_sort(joueur_sort, joueur, adversaire);
                        }
                    }

                    SDL_RenderClear(renderer);
                    afficher_personnages_sdl(renderer, font, joueur, adversaire);
                    SDL_RenderPresent(renderer);

                    SDL_Delay(1000); 
                }
            }
        }
    }

    if (est_vivant(joueur)) {
        printf("\n Vous avez gagné !\n");
    } else {
        printf("\n Vous avez perdu !\n");
    }
}

// Créer des statistiques de personnage
Stats creer_stats(int vie, int attaque, int defense, int vitesse) {
    Stats stats = {vie, attaque, defense, vitesse};
    return stats;
}

// Créer un personnage
Personnage creer_personnage(const char *nom, Stats stats_base) {
    Personnage perso;
    strcpy(perso.nom, nom);
    perso.base = stats_base;
    perso.actuel = stats_base;
    return perso;
}

// Choisir l'action de l'adversaire
SortId choisir_action_adversaire(const Personnage *adversaire, const Personnage *joueur) {
    if (adversaire->actuel.vie < 30) {
        return SORT_SOIN;
    }
    if (adversaire->actuel.vitesse < joueur->actuel.vitesse) {
        return (rand() % 3 < 2) ? SORT_AUGMENTE_VITESSE : SORT_REDUIT_VITESSE;
    }
    if (adversaire->actuel.attaque < joueur->actuel.defense) {
        return (rand() % 3 < 2) ? SORT_AUGMENTE_ATTAQUE : SORT_REDUIT_ATTAQUE;
    }
    if (adversaire->actuel.defense < joueur->actuel.attaque) {
        return SORT_AUGMENTE_DEFENSE;
    }
    return SORT_COUP;
}

// Afficher les personnages dans SDL
void afficher_personnages_sdl(SDL_Renderer *renderer, TTF_Font *font, const Personnage *joueur, const Personnage *adversaire) {
    SDL_Color blanc = {255, 255, 255};
    char buffer[256];

    sprintf(buffer, "%s      VS        %s", joueur->nom, adversaire->nom);
    afficher_texte(renderer, font, buffer, 200, 50, blanc);

    sprintf(buffer, "Vie : %3d | Vie : %3d", joueur->actuel.vie, adversaire->actuel.vie);
    afficher_texte(renderer, font, buffer, 200, 100, blanc);

    sprintf(buffer, "Attaque : %3d | Attaque : %3d", joueur->actuel.attaque, adversaire->actuel.attaque);
    afficher_texte(renderer, font, buffer, 200, 150, blanc);

    sprintf(buffer, "Defense : %3d | Defense : %3d", joueur->actuel.defense, adversaire->actuel.defense);
    afficher_texte(renderer, font, buffer, 200, 200, blanc);

    sprintf(buffer, "Vitesse : %3d | Vitesse : %3d", joueur->actuel.vitesse, adversaire->actuel.vitesse);
    afficher_texte(renderer, font, buffer, 200, 250, blanc);
}

// Afficher les sorts dans SDL
void afficher_sorts_sdl(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color blanc = {255, 255, 255};

    afficher_texte(renderer, font, "1. Coup", 100, 350, blanc);
    afficher_texte(renderer, font, "2. Réduire attaque", 100, 400, blanc);
    afficher_texte(renderer, font, "3. Réduire défense", 100, 450, blanc);
    afficher_texte(renderer, font, "4. Réduire vitesse", 100, 500, blanc);
    afficher_texte(renderer, font, "5. Soin", 400, 350, blanc);
    afficher_texte(renderer, font, "6. Augmenter attaque", 400, 400, blanc);
    afficher_texte(renderer, font, "7. Augmenter défense", 400, 450, blanc);
    afficher_texte(renderer, font, "8. Augmenter vitesse", 400, 500, blanc);
}

// Appliquer un sort
void appliquer_sort(SortId id, Personnage *lanceur, Personnage *cible) {
    switch (id) {
        case SORT_COUP: {
            int degats = max(lanceur->actuel.attaque / max(cible->actuel.defense, 1), 1);
            cible->actuel.vie = max(cible->actuel.vie - degats, 0);
            printf("%s attaque %s pour %d dégâts.\n", lanceur->nom, cible->nom, degats);
            break;
        }
        case SORT_REDUIT_ATTAQUE:
            cible->actuel.attaque = max(cible->actuel.attaque * 0.8, 1);
            printf("L'attaque de %s diminue.\n", cible->nom);
            break;
        case SORT_REDUIT_DEFENSE:
            cible->actuel.defense = max(cible->actuel.defense * 0.8, 1);
            printf("La défense de %s diminue.\n", cible->nom);
            break;
        case SORT_REDUIT_VITESSE:
            cible->actuel.vitesse = max(cible->actuel.vitesse * 0.8, 1);
            printf("La vitesse de %s diminue.\n", cible->nom);
            break;
        case SORT_SOIN:
            lanceur->actuel.vie = min(lanceur->actuel.vie + 20, lanceur->base.vie);
            printf("%s se soigne.\n", lanceur->nom);
            break;
        case SORT_AUGMENTE_ATTAQUE:
            lanceur->actuel.attaque *= 1.2;
            printf("L'attaque de %s augmente.\n", lanceur->nom);
            break;
        case SORT_AUGMENTE_DEFENSE:
            lanceur->actuel.defense *= 1.2;
            printf("La défense de %s augmente.\n", lanceur->nom);
            break;
        case SORT_AUGMENTE_VITESSE:
            lanceur->actuel.vitesse *= 1.1;
            printf("La vitesse de %s augmente.\n", lanceur->nom);
            break;
        default:
            printf("Action invalide.\n");
    }
}

// Vérifier si un personnage est vivant
int est_vivant(const Personnage *perso) {
    return perso->actuel.vie > 0;
}
