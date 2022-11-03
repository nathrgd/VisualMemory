#ifndef PILE_H
#define PILE_H

#include <stdbool.h>
#include "case.h"
#include "pile.h"


typedef struct pile_s pile;
struct pile_s {
    case_t *sommet;
    pile *suivant;
};


bool pile_est_vide(pile *p);
pile * pile_vide();
pile * empiler(case_t *c, pile *p);
case_t *sommet(pile *p);
case_t *depiler(pile **p);
void pile_liberer(pile *p);


#endif
