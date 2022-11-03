#ifndef CASE_H
#define CASE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "case.h"


#define H_CASE 80
#define W_CASE 80


typedef enum couleur_e {BLANC, BLEU, NOIR} couleur_t;


typedef struct case_s {
    bool cliquable;
    couleur_t couleur;
    SDL_Rect dstrect;
    SDL_Texture *texture;
} case_t;


bool est_cliquable(case_t c);
void set_cliquable(case_t *c, bool b);
void set_couleur(case_t *c, couleur_t couleur);
void set_texture(case_t *c, SDL_Texture *texture);
void set_dstrect(case_t *c, SDL_Rect dstrect);
couleur_t get_couleur(case_t c);
bool cases_correspondent(case_t *c1, case_t *c2);

#endif
