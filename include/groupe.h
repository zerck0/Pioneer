#ifndef GROUPE_H
#define GROUPE_H

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

void initialiserGroupe(Groupe *groupe);
void avancer(Groupe *groupe);
void chercherNourriture(Groupe *groupe);
void chercherEau(Groupe *groupe);
void seReposer(Groupe *groupe);
void afficherEtat(Groupe *groupe);
void miseAJourEtat(Groupe *groupe);
void afficherMessageAccueil();
void sauvegarderPartie(Groupe *groupe);
int chargerPartie(Groupe *groupe, const char *chemin);
int menuPrincipal(Groupe *groupe);
void jouer(Groupe *groupe);

#endif