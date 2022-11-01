#ifndef CASE_H
#define CASE_H

#include <stdbool.h>
#include "case.h"


#define H_CASE 80
#define W_CASE 80


typedef enum couleur_e {BLANC, BLEU, NOIR} couleur_t;


typedef struct case_s {
    int valeur;
    bool cliquable;
    couleur_t couleur;
} case_t;


case_t nouvelle_case(int v);
bool est_cliquable(case_t c);
void set_cliquable(case_t *c, bool b);
void set_couleur(case_t *c, couleur_t couleur);
int get_valeur(case_t c);
bool cases_correspondent(case_t c1, case_t c2);

#endif
