#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

// Définition de la structure Groupe
typedef struct {
    int colons;
    int sante;
    int puissanceAttaque;
    int experience;
    int nourriture;
    int eau;
    int fatigue;
    int joursSansEau;
    int joursSansNourriture;
    int joursPasses;
    int distanceRestante;
    int vitesse;
    int temperature;
} Groupe;

// Définition de la structure des actions du jour
typedef struct {
    int boire;
    int manger;
    int avancer;
    int se_reposer;
    int chasser;
    int chercher_eau;
} ActionsJour;

// Fonctions principales
void init_groupe(Groupe *groupe);
void init_actions_jour(ActionsJour *actions);
void appliquer_actions(Groupe *groupe, ActionsJour *actions);
void declencher_evenement(Groupe *groupe); // Ajout de la déclaration ici

#endif
