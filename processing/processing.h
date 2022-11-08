#ifndef PROCESSING_H
#define PROCESSING_H

#include <SDL2/SDL.h>
#include "processing.h"
#include "../type/case.h"
#include "../type/file.h"

case_t **create_tab_cases(int n, int m);
void free_tab_cases(case_t **tab, int n);
void render_copy(SDL_Renderer *renderer, case_t **tab_cases, int n, int m);
void reset_cases(case_t **tab_cases, int n, int m, SDL_Texture *texture_def);
void selectionner_et_montrer_cases(case_t **tab_cases, int n, int m, int nb_selections,
                                   SDL_Renderer *renderer, SDL_Texture *texture_case_blanche, file *p);
case_t * recuperer_case(case_t **tab_cases, int n, int m, int xCursor, int yCursor);

#endif
