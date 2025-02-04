#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    actions->chercher_nourriture = 0;
}

// Appliquer les actions du joueur
void appliquer_actions(Groupe *groupe, ActionsJour *actions) {
    // Gestion de l'eau et de la nourriture
    if (actions->boire && groupe->eau > 0) {
        groupe->eau -= 5;
        groupe->sante += 5;
    } else {
        groupe->joursSansEau++;
    }

    if (actions->manger && groupe->nourriture > 0) {
        groupe->nourriture -= 5;
        groupe->sante += 5;
    } else {
        groupe->joursSansNourriture++;
    }

    // Avancer
    if (actions->avancer) {
        groupe->distanceRestante -= groupe->vitesse;
        groupe->fatigue += 10;
    }

    // Se reposer
    if (actions->se_reposer) {
        groupe->fatigue -= 10;
        if (groupe->fatigue < 0) groupe->fatigue = 0;
    }

    // Chercher de l'eau
    if (actions->chercher_eau && rand() % 2 == 0) {
        groupe->eau += 10;
    }

    // Chercher de la nourriture
    if (actions->chercher_nourriture && rand() % 2 == 0) {
        groupe->nourriture += 10;
    }

    // Vérification de la survie
    if (groupe->joursSansEau > 3) {
        groupe->sante -= 20;
    }
    if (groupe->joursSansNourriture > 5) {
        groupe->sante -= 10;
    }
    if (groupe->fatigue >= 100) {
        groupe->sante -= 5;
    }

    // Passage au jour suivant
    groupe->joursPasses++;

    // Réinitialisation des actions du jour
    init_actions_jour(actions);
}
