#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/groupe.h"  // Inclure le fichier d'en-tête

#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

// Initialisation du groupe
void initialiserGroupe(Groupe *groupe) {
    groupe->colons = 10;
    groupe->sante = 100;
    groupe->puissanceAttaque = 50;
    groupe->experience = 0;
    groupe->nourriture = 100;
    groupe->eau = 100;
    groupe->fatigue = 0;
    groupe->joursSansEau = 0;
    groupe->joursSansNourriture = 0;
    groupe->joursPasses = 0;
    groupe->distanceRestante = 200;
    groupe->vitesse = 10;
    groupe->temperature = 30;
}

void avancer(Groupe *groupe);
void chercherNourriture(Groupe *groupe);
void chercherEau(Groupe *groupe);
void seReposer(Groupe *groupe);

// Affichage de l'état du groupe
void afficherEtat(Groupe *groupe) {
    printf("\nÉtat actuel du groupe :\n");
    printf("Colons : %d\n", groupe->colons);
    printf("Santé : %d\n", groupe->sante);
    printf("Puissance d'attaque : %d\n", groupe->puissanceAttaque);
    printf("Expérience : %d\n", groupe->experience);
    printf("Nourriture : %d\n", groupe->nourriture);
    printf("Eau : %d\n", groupe->eau);
    printf("Fatigue : %d\n", groupe->fatigue);
    printf("Température : %d°C\n", groupe->temperature);
    printf("Jours passés : %d\n", groupe->joursPasses);
    printf("Jours sans eau : %d\n", groupe->joursSansEau);
    printf("Jours sans nourriture : %d\n", groupe->joursSansNourriture);
    printf("Distance restante : %d km\n", groupe->distanceRestante);
    printf("Vitesse actuelle : %d km/jour\n", groupe->vitesse);
}

void miseAJourEtat(Groupe *groupe) {
    groupe->temperature = 25 + rand() % 20;

    if (groupe->temperature > 35) {
        groupe->fatigue += 10;
        groupe->eau -= 20;
        groupe->sante -= 5;
        printf("\nLa température élevée affecte la santé et consomme plus d'eau.\n");
    } else {
        groupe->eau -= 10;
    }

    if (groupe->eau < 0) groupe->eau = 0;
    if (groupe->nourriture < 0) groupe->nourriture = 0;
    if (groupe->colons < 0) groupe->colons = 0;
    if (groupe->sante < 0) groupe->sante = 0;

    if (groupe->eau == 0) {
        groupe->joursSansEau++;
        if (groupe->joursSansEau > 2) {
            printf("\n[ALERTE] Manque d'eau prolongé ! Des colons sont morts.\n");
            groupe->colons -= 2;
            groupe->sante -= 20;
            if (groupe->colons <= 0 || groupe->sante <= 0) {
                printf("\n[GAME OVER] Le groupe n'a pas survécu.\n");
                exit(0);  
            }
        }
    } else {
        groupe->joursSansEau = 0; 
    }

    if (groupe->nourriture == 0) {
        groupe->joursSansNourriture++;
        if (groupe->joursSansNourriture > 6) {
            printf("\n[ALERTE] Manque de nourriture prolongé ! Des colons sont morts.\n");
            groupe->colons -= 2;
            groupe->sante -= 20;
            if (groupe->colons <= 0 || groupe->sante <= 0) {
                printf("\n[GAME OVER] Le groupe n'a pas survécu.\n");
                exit(0);  
            }
        }
    } else {
        groupe->joursSansNourriture = 0; 
    }

    groupe->experience += 2;
    groupe->puissanceAttaque = 50 + groupe->experience / 10;

    groupe->distanceRestante -= groupe->vitesse;
    if (groupe->distanceRestante < 0) groupe->distanceRestante = 0;
}

// Affichage de message d'accueil
void afficherMessageAccueil() {
    system(CLEAR);
    printf("\n");
    printf("******************************************************************\n");
    printf("*                                                                *\n");
    printf("*                   Bienvenue sur Tatooine Survival              *\n");
    printf("*                                                                *\n");
    printf("*        Une aventure de survie dans le désert impitoyable       *\n");
    printf("*                                                                *\n");
    printf("******************************************************************\n");
    printf("\n");
    printf("1. Nouvelle Partie\n");
    printf("2. Reprendre une Partie\n");
    printf("3. Quitter\n");
    printf("Choisissez une option : ");
}

// Sauvegarde de la partie
void sauvegarderPartie(Groupe *groupe) {
    char chemin[100];
    snprintf(chemin, sizeof(chemin), "C:/Users/aiman/Downloads/Tatooine Sim/Saves/save_%d.txt", groupe->joursPasses);
    FILE *fichier = fopen(chemin, "w");

    if (fichier) {
        fprintf(fichier, "%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
            groupe->colons, groupe->sante, groupe->puissanceAttaque, groupe->experience,
            groupe->nourriture, groupe->eau, groupe->fatigue, groupe->joursSansEau,
            groupe->joursSansNourriture, groupe->joursPasses, groupe->distanceRestante,
            groupe->vitesse, groupe->temperature);
        fclose(fichier);
        printf("\nPartie sauvegardée dans : %s\n", chemin);
    } else {
        printf("Erreur : impossible de sauvegarder la partie.\n");
    }
}

// Chargement de la partie
int chargerPartie(Groupe *groupe, const char *chemin) {
    FILE *fichier = fopen(chemin, "r");

    if (fichier) {
        fscanf(fichier, "%d %d %d %d %d %d %d %d %d %d %d %d %d",
            &groupe->colons, &groupe->sante, &groupe->puissanceAttaque, &groupe->experience,
            &groupe->nourriture, &groupe->eau, &groupe->fatigue, &groupe->joursSansEau,
            &groupe->joursSansNourriture, &groupe->joursPasses, &groupe->distanceRestante,
            &groupe->vitesse, &groupe->temperature);
        fclose(fichier);
        return 1;
    } else {
        printf("Erreur : fichier de sauvegarde non trouvé.\n");
        return 0;
    }
}

// Menu principal
int menuPrincipal(Groupe *groupe) {
    int choix;
    afficherMessageAccueil();
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            initialiserGroupe(groupe);
            return 1;
        case 2: {
            char chemin[100];
            printf("\nEntrez le chemin du fichier de sauvegarde : ");
            scanf("%s", chemin);
            if (chargerPartie(groupe, chemin)) {
                return 1;
            }
            return 0;
        }
        case 3:
            printf("\nMerci d'avoir joué. À bientôt !\n");
            exit(0);
        default:
            printf("Choix invalide. Veuillez réessayer.\n");
            return 0;
    }
}

// DEMMARAGE DU JEU ICI
void jouer(Groupe *groupe) {
    int choix;
    while (groupe->distanceRestante > 0) {
        system(CLEAR);
        afficherEtat(groupe);
        printf("\nActions disponibles :\n");
        printf("1. Continuer d'avancer\n");
        printf("2. Chercher de la nourriture\n");
        printf("3. Chercher de l'eau\n");
        printf("4. Se reposer\n");
        printf("5. Sauvegarder la partie\n");
        printf("\nChoisissez une action : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                avancer(groupe);
                break;
            case 2:
                chercherNourriture(groupe);
                break;
            case 3:
                chercherEau(groupe);
                break;
            case 4:
                seReposer(groupe);
                break;
            case 5:
                sauvegarderPartie(groupe);
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
        }
    }
    printf("\n[FELICITATIONS] Vous avez atteint la destination !\n");
}

// Avancer le groupe
void avancer(Groupe *groupe) {
    groupe->fatigue += 10;
    groupe->eau -= 15;
    groupe->nourriture -= 10;
    groupe->joursPasses++;
    printf("\nLe groupe avance, mais la fatigue, la nourriture, et l'eau diminuent.\n");
}

// Recherche de nourriture
void chercherNourriture(Groupe *groupe) {
    int nourritureTrouvee = rand() % 30 + 10;
    groupe->nourriture += nourritureTrouvee;
    groupe->fatigue += 5;
    groupe->joursPasses++;
    groupe->experience += 5;
    printf("\nLe groupe cherche et trouve %d unités de nourriture.\n", nourritureTrouvee);
}

// Recherche d'eau
void chercherEau(Groupe *groupe) {
    int eauTrouvee = rand() % 20 + 10;
    groupe->eau += eauTrouvee;
    groupe->fatigue += 5;
    groupe->joursPasses++;
    groupe->experience += 5;
    printf("\nLe groupe cherche et trouve %d unités d'eau.\n", eauTrouvee);
}

// Repos
void seReposer(Groupe *groupe) {
    groupe->fatigue -= 15;
    if (groupe->fatigue < 0) groupe->fatigue = 0;
    groupe->sante += 10;
    groupe->joursPasses++;
    printf("\nLe groupe se repose et réduit sa fatigue.\n");
}

int main() {
    srand(time(NULL));
    Groupe groupe;

    while (!menuPrincipal(&groupe)) {}

    jouer(&groupe);

    return 0;
}