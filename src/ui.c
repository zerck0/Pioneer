#include "../include/ui.h"
#include "../include/renderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

// Déclaration des variables externes
extern int fullscreen;
extern int music_on;
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font;
extern SDL_Color blanc;

// Déclaration des fonctions externes
extern void toggle_fullscreen();
extern void toggle_music();
extern void afficher_options_menu(int *running);

// Fonction pour créer une texture texte
SDL_Texture* create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect) {
    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, color);
    if (!surface) {
        printf("Erreur TTF_RenderText_Solid : %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Erreur SDL_CreateTextureFromSurface : %s\n", SDL_GetError());
    }

    rect->w = surface->w;
    rect->h = surface->h;

    SDL_FreeSurface(surface);
    return texture;
}

// Fonction pour afficher du texte
void afficher_texte(SDL_Renderer *renderer, TTF_Font *font, const char *texte, int x, int y, SDL_Color couleur) {
    SDL_Surface *surface = TTF_RenderText_Blended(font, texte, couleur);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}