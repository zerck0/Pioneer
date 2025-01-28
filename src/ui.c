#include "../include/ui.h"
#include <stdio.h>

SDL_Texture* create_text_texture(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, SDL_Rect *rect) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
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
