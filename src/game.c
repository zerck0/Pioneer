#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>

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
    groupe->distanceRestante = 500; // Par exemple, 500 km avant l'objectif
    groupe->vitesse = 10; // Km/jour
    groupe->temperature = 35; // Exemple de température désertique
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
        groupe->fatigue = 100; // Fatigue ne dépasse pas 100
    }

    // Vérification de la survie
    if (groupe->sante <= 0 || groupe->colons <= 0) {
        printf("Game Over ! Votre groupe n’a pas survécu...\n");
        exit(0);
    }
}

// Affichage des statistiques du groupe dans la console
void afficher_groupe_console(Groupe *groupe) {
    printf("Jour %d | Colons: %d | Santé: %d | Attaque: %d | Expérience: %d\n",
           groupe->joursPasses, groupe->colons, groupe->sante, groupe->puissanceAttaque, groupe->experience);
    printf("Nourriture: %d | Eau: %d | Fatigue: %d | Température: %d°C\n",
           groupe->nourriture, groupe->eau, groupe->fatigue, groupe->temperature);
    printf("Jours sans eau: %d | Jours sans nourriture: %d\n",
           groupe->joursSansEau, groupe->joursSansNourriture);
    printf("Distance restante: %d km\n\n", groupe->distanceRestante);
}
