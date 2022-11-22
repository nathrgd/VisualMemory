#ifndef FICHIERS_H
#define FICHIERS_H

#include "fichiers.h"

#define MAX_WEIGHT 50
#define MAX_HEIGHT 50


int taille_sans_espaces_et_fin_lignes(const char *chaine);
void lire_dimensions(const char *nomFichier, int *w, int *h);
void ecrire(const char *nomFichier, const char *contenu);
void ecrire_en_debut(const char *nomFichier, const char *contenu);


#endif
