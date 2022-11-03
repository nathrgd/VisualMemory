#include <stdio.h>
#include "case.h"


bool est_cliquable(case_t c) {
    return c.cliquable;
}

void set_cliquable(case_t *c, bool b) {
    c->cliquable = b;
}

void set_couleur(case_t *c, couleur_t couleur) {
    c->couleur = couleur;
}

void set_texture(case_t *c, SDL_Texture *texture) {
    c->texture = texture;
}

void set_dstrect(case_t *c, SDL_Rect dstrect) {
    c->dstrect = dstrect;
}

couleur_t get_couleur(case_t c) {
    return c.couleur;
}

bool cases_correspondent(case_t *c1, case_t *c2) {
    return c1 == c2;
}
