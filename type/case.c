#include <stdio.h>
#include "case.h"


case_t nouvelle_case(int v) {
    case_t c = {v, true, BLEU};
    return c;
}

bool est_cliquable(case_t c) {
    return c.cliquable;
}

void set_cliquable(case_t *c, bool b) {
    c->cliquable = b;
}

void set_couleur(case_t *c, couleur_t couleur) {
    c->couleur = couleur;
}

int get_valeur(case_t c) {
    return c.valeur;
}

bool cases_correspondent(case_t c1, case_t c2) {
    return get_valeur(c1) == get_valeur(c2);
}
