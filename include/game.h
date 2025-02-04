#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

// Structure représentant un groupe de colons
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

// Structure représentant les actions choisies pour la journée
typedef struct {
    int boire;
    int manger;
    int avancer;
    int se_reposer;
    int chercher_eau;
    int chercher_nourriture;
} ActionsJour;

// Initialisation du groupe
void init_groupe(Groupe *groupe);

// Initialisation des actions du jour
void init_actions_jour(ActionsJour *actions);

// Vérifier et appliquer les actions du joueur
void appliquer_actions(Groupe *groupe, ActionsJour *actions);

#endif
