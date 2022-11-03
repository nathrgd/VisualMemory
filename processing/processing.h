#ifndef PROCESSING_H
#define PROCESSING_H

#include <SDL2/SDL.h>
#include "processing.h"
#include "../type/case.h"

case_t **create_tab_cases(int n, int m);
void free_tab_cases(case_t **tab, int n);
void render_copy(SDL_Renderer *renderer, case_t **tab_cases, int n, int m);
void reset_cases(case_t **tab_cases, int n, int m, SDL_Texture *texture_def);

#endif
