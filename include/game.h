#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

// Définition des structures de données
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

typedef struct {
    int boire;
    int manger;
    int avancer;
    int se_reposer;
    int chasser;
    int chercher_eau;
} ActionsJour;

typedef struct {
    int vie;
    int attaque;
    int defense;
    int vitesse;
} Stats;

typedef struct {
    char nom[20];
    Stats base;
    Stats actuel;
} Personnage;

typedef enum {
    STAT_VIE,
    STAT_ATTAQUE,
    STAT_DEFENSE,
    STAT_VITESSE,
    STAT_AUCUNE
} StatId;

typedef enum {
    SORT_COUP = 1,
    SORT_REDUIT_ATTAQUE,
    SORT_REDUIT_DEFENSE,
    SORT_REDUIT_VITESSE,
    SORT_SOIN,
    SORT_AUGMENTE_ATTAQUE,
    SORT_AUGMENTE_DEFENSE,
    SORT_AUGMENTE_VITESSE,
    SORT_AUCUNE
} SortId;

// Déclaration des fonctions
void init_groupe(Groupe *groupe);
void init_actions_jour(ActionsJour *actions);
void appliquer_actions(Groupe *groupe, ActionsJour *actions);
void declencher_evenement(Groupe *groupe);
void save_game(Groupe *groupe);
int load_game(const char *filepath, Groupe *groupe);

Stats creer_stats(int vie, int attaque, int defense, int vitesse);
Personnage creer_personnage(const char *nom, Stats stats_base);
SortId choisir_action_adversaire(const Personnage *adversaire, const Personnage *joueur);
void afficher_personnages(const Personnage *joueur, const Personnage *adversaire);
void afficher_sorts();
void appliquer_sort(SortId id, Personnage *lanceur, Personnage *cible);
int est_vivant(const Personnage *perso);
void combat(Personnage *joueur, Personnage *adversaire);
void combat_sdl(SDL_Renderer *renderer, TTF_Font *font, Personnage *joueur, Personnage *adversaire);

#endif
