#include "../include/game.h"
#include "../include/renderer.h"
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
    actions->chasser = 0;
}

// Appliquer les actions du joueur
void appliquer_actions(Groupe *groupe, ActionsJour *actions) {
    // Si le joueur marche
    if (actions->avancer) {
        groupe->distanceRestante -= 10;
        groupe->eau -= 6;
        groupe->nourriture -= 5;
        groupe->fatigue += 8;
    }

    // Si le joueur boit
    if (actions->boire && groupe->eau > 0) {
        groupe->eau -= 5;
    }

    // Si le joueur mange
    if (actions->manger && groupe->nourriture > 0) {
        groupe->nourriture -= 5;
    }

    // Si le joueur se repose
    if (actions->se_reposer) {
        groupe->fatigue -= 15;
        if (groupe->fatigue < 0) groupe->fatigue = 0;
    }

    // **Nouvelle action : Chasser**
    if (actions->chasser) {
        if (rand() % 2 == 0) {
            groupe->nourriture += 15; // Réussite : gain de nourriture
        } else {
            groupe->sante -= 5; // Échec : blessure
        }
    }

    // **Nouvelle action : Chercher de l'eau**
    if (actions->chercher_eau) {
        if (rand() % 2 == 0) {
            groupe->eau += 15; // Réussite : gain d’eau
        } else {
            groupe->fatigue += 5; // Échec : fatigue
        }
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
        groupe->sante -= 20;  // Déshydratation
    }
    
    if (groupe->joursSansNourriture >= 5) {
        groupe->sante -= 10;  // Malnutrition
    }

    if (groupe->fatigue >= 90) {
        groupe->vitesse = 5;  // Fatigue extrême ralentit la progression
    } else {
        groupe->vitesse = 10; // Sinon, vitesse normale
    }

    // Passage au jour suivant
    groupe->joursPasses++;

    // **Tous les 3 jours, un événement se produit**
    if (groupe->joursPasses % 3 == 0) {
        declencher_evenement(groupe);
    }

    // Vérification de la survie
    if (groupe->sante <= 0 || groupe->colons <= 0) {
        printf("Perdu ! Votre colonie n'a pas survécu.\n");
    }

    // Réinitialisation des actions
    init_actions_jour(actions);
}


void declencher_evenement(Groupe *groupe) {
    int choix_evenement = rand() % 4; // Choisir un événement aléatoire parmi 3
    const char *image_path;

    switch (choix_evenement) {
        case 0:
            image_path = "assets/images/oldman.jpg";
            afficher_evenement_sdl(groupe, image_path, 
                "Un vieil homme propose de partager ses rations en échange d’eau. Acceptez-vous ?", 
                10, -5, // Accepter : +10 nourriture, -5 eau
                0, 0    // Refuser : Rien ne se passe
            );
            break;

        case 1:
            image_path = "assets/images/sandstorm.jpg";
            afficher_evenement_sdl(groupe, image_path, 
                "Une tempête de sable approche ! On vous propose de vous cacher dans une grotte. Que faites-vous ?", 
                0, -5,   // Se cacher : -5 fatigue
                -10, -3  // Continuer : -10 santé, -3 eau
            );
            break;

        case 2:
            image_path = "assets/images/camp.jpg";
            afficher_evenement_sdl(groupe, image_path,
                "Vous trouvez un camp abandonné. Voulez-vous fouiller ?", 
                15, 10,  // Fouiller : +15 nourriture, +10 eau
                0, 0     // Ignorer : Rien ne se passe
            );
            break;
        
        case 3:  
            image_path = "assets/images/trancho.jpg"; // Image à utiliser pour l'événement
            afficher_evenement_sdl(groupe, image_path,  
                "Un homme mystérieux au nom de Trancho vous propose de l'aide. Que faire ?",  
                10, 5,  // Accepter : +5 nourriture, +5 eau  
                -20, 0  // Refuser : -20 santé 
            );  
            break;

    }
}
