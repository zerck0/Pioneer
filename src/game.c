#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Initialisation du groupe de colons
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

// Mise à jour quotidienne du groupe
void mise_a_jour_groupe(Groupe *groupe) {
    groupe->joursPasses++;
    groupe->nourriture -= 5;
    groupe->eau -= 3;
    groupe->fatigue += 2;
    groupe->distanceRestante -= groupe->vitesse;

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

    // Conditions de survie
    if (groupe->joursSansEau > 3) {
        groupe->sante -= 20;
    }
    if (groupe->joursSansNourriture > 5) {
        groupe->sante -= 10;
    }
    if (groupe->fatigue >= 100) {
        groupe->sante -= 5;
        groupe->fatigue = 100;
    }

    // Vérification de la survie
    if (groupe->sante <= 0 || groupe->colons <= 0) {
        printf("Game Over ! Votre groupe n’a pas survécu...\n");
        exit(0);
    }
}

void choix_joueur(Groupe *groupe, int choix) {
    srand(time(NULL));
    switch (choix) {
        case 1: // Avancer
            groupe->distanceRestante -= groupe->vitesse;
            groupe->nourriture -= 5;
            groupe->eau -= 5;
            groupe->fatigue += 5;
            break;
        
        case 2: // Chercher de l'eau
            if (rand() % 2 == 0) {
                groupe->eau += 20;
            }
            break;

        case 3: // Chercher de la nourriture
            if (rand() % 2 == 0) {
                groupe->nourriture += 15;
            } else {
                groupe->sante -= 5;
            }
            break;

        case 4: // Se reposer
            groupe->fatigue -= 10;
            if (groupe->fatigue < 0) groupe->fatigue = 0;
            break;

        case 5: // Passer un jour sans action
            break;

        default:
            break;
    }
}