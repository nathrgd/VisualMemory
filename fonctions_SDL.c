/**
 * @file fonctions_SDL.c
 * @author Nathan Regaudie
 */

#include <stdio.h>
#include <stdlib.h>
#include "fonctions_SDL.h"



/**
 * @brief Initialise la SDL, crée une fenêtre et un renderer.
 *        Si une erreur est rencontrée, la fonction quitte le programme.
 * 
 * @param window fenêtre du jeu
 * @param renderer renderer
 * @param weight largeur de la fenêtre
 * @param height hauteur de la fenêtre
 */
void init_SDL(SDL_Window **window, SDL_Renderer **renderer, int weight, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        printf("Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        return exit(EXIT_FAILURE);
    }

    *window = SDL_CreateWindow("VisualMemory", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, weight, height, SDL_WINDOW_SHOWN);
    if (*window == NULL) {
        printf("Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL) {
        printf("Erreur lors de la création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}



SDL_Texture * charger_image (const char* nomfichier, SDL_Renderer* renderer)
{
    SDL_Surface * surface = SDL_LoadBMP(nomfichier);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    return texture;
}

SDL_Texture * charger_image_transparente(const char* nomfichier,
                                       SDL_Renderer* renderer,
                                       Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Surface * surface = SDL_LoadBMP(nomfichier);
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

    return texture;
}

SDL_Texture * charger_texte(const char* message, SDL_Renderer* renderer,
                            TTF_Font *font, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, message, color);
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    return texture;
}
