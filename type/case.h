#ifndef CASE_H
#define CASE_H

#include <stdbool.h>
#include "case.h"

typedef struct case_s {
    int valeur;
    bool cliquable;
} case_t;


case_t nouvelle_case(int v);
bool est_cliquable(case_t c);
void set_cliquable(case_t *c, bool b);
int get_valeur(case_t c);
bool cases_correspondent(case_t c1, case_t c2);

#endif
