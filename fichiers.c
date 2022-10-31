#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "fichiers.h"



/**
 * @brief Retourne la taille d'une chaine de caractères en ne prenant pas en compte les espaces et les caractères de fin de ligne.
 * 
 * @param chaine chaine de caractère
 * @return taille de la chaine sans espaces et caractères de fin de ligne
 */
int taille_sans_espaces_et_fin_lignes(const char *chaine) {
    char c;
    int n = 0;
    for (int i = 0; chaine[i] != '\0'; i++) {
        c = chaine[i];
        if (c != ' ' && c != '\r' && c != '\n')
            n++;
    }
    return n;
}


/**
 * @brief Lis le fichier d'entrée pour en déduire les dimensions de la grille de jeu
 * 
 * @param nomFichier nom du fichier d'entrée
 * @param w largeur
 * @param h hauteur
 */
void lire_dimensions(const char *nomFichier, int *w, int *h) {
    FILE *fic = fopen(nomFichier, "r");
    int n;
    char buffer[2*MAX_WEIGHT+2]; // 2*MAX_WEIGHT+2 pour prendre en compte les possibles espaces et les caractères de fin de ligne

    if (fic == NULL) {
        printf("Erreur lors de l'ouverture de '%s' : application des dimensions par défaut (10x10)\n", nomFichier);
        *w = 10;
        *h = 10;
    } else {
        *w = 0;
        *h = 0;
        while (fgets(buffer, 2*MAX_WEIGHT+2, fic) != NULL && *h <= 50) {
            n = taille_sans_espaces_et_fin_lignes(buffer);
            if (n > 0)
                (*h)++;
            if (n > *w)
                *w = n;
        }
    }
}
