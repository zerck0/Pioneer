#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

// Structure représentant un groupe de colons en survie
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

// Initialisation du groupe de colons
void init_groupe(Groupe *groupe);

// Mise à jour des stats du groupe (chaque jour qui passe)
void mise_a_jour_groupe(Groupe *groupe);

// Affichage des stats du groupe dans la console
void afficher_groupe_console(Groupe *groupe);

#endif
