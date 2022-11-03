#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "case.h"
#include "pile.h"


pile * pile_vide() {
    return NULL;
}

pile * empiler(case_t *c, pile *p) {
    pile * res = (pile *)malloc(sizeof(pile));
    res->sommet = c;
    res->suivant = p;
    return res;
}

bool pile_est_vide(pile *p) {
    return p == NULL;
}

case_t *sommet(pile *p) {
    if (pile_est_vide(p)) {
        printf("Erreur: sommet(pile p) : p est vide\n");
        exit(EXIT_FAILURE);
    }
    return p->sommet;
}

case_t *depiler(pile **p) {
    pile *tmp;
    case_t *s;
    if (pile_est_vide(*p)) {
        printf("Erreur: depiler(pile p) : p est vide\n");
        exit(EXIT_FAILURE);
    }
    s = sommet(*p);
    tmp = *p;
    *p = (*p)->suivant;
    free(tmp);
    return s;
}

void pile_liberer(pile *p) {
    if (!pile_est_vide(p)) {
        pile_liberer(p->suivant);
        free(p);
    }
}
