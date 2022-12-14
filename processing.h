#ifndef PROCESSING_H
#define PROCESSING_H

#include <SDL2/SDL.h>
#include "processing.h"
#include "jeu.h"
#include "type/case.h"
#include "type/file.h"

case_t **create_tab_cases(int n, int m);
void free_tab_cases(case_t **tab, int n);
void afficher_cases(SDL_Renderer *renderer, case_t **tab_cases, int n, int m);
void afficher_vies(SDL_Renderer *renderer, int nb_vies, SDL_Texture *texture_coeur);
void reset_cases(case_t **tab_cases, int n, int m, SDL_Texture *texture_def);
void selectionner_et_montrer_cases(jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer);
case_t * recuperer_case(case_t **tab_cases, int n, int m, int xCursor, int yCursor);
char * get_texte_pour_historique(int manche, int score);

#endif
