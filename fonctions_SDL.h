/**
 * @file fonctions_SDL.h
 * @author Nathan Regaudie
 */

#ifndef FONCTIONS_SDL_H
#define FONCTIONS_SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


void init_SDL(SDL_Window **window, SDL_Renderer **renderer, int weight, int height);
SDL_Texture * charger_image (const char* nomfichier, SDL_Renderer* renderer); 
SDL_Texture * charger_image_transparente(const char* nomfichier, SDL_Renderer* renderer,
                                       Uint8 r, Uint8 g, Uint8 b);
SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer,
                           TTF_Font *font, SDL_Color color);
void update_message_text(const char *new_message, SDL_Texture **message_texture, SDL_Renderer *renderer,
                         TTF_Font *font, SDL_Color color);

#endif