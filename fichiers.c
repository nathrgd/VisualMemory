#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "fichiers.h"

#define W_DEF 8
#define H_DEF 8



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
        printf("Erreur lors de l'ouverture de '%s' : application des dimensions par défaut (%dx%d)\n", nomFichier, W_DEF, H_DEF);
        *w = W_DEF;
        *h = H_DEF;
    } else {
        *w = 0;
        *h = 0;
        while (fgets(buffer, 2*MAX_WEIGHT+2, fic) != NULL && *h < 6) {
            n = taille_sans_espaces_et_fin_lignes(buffer);
            if (n > 0)
                (*h)++;
            if (n > 6)
                n = 6;
            if (n > *w)
                *w = n;
        }


        // Les dimensions minimales sont de 3x3
        if (*w < 3) {
            printf("'%s' : La largeur de la grille est strict. inférieure à 3 : application de la largeur minimale (3)\n", nomFichier);
            *w = 3;
        }
        if (*h < 3) {
            printf("'%s' : La hauteur de la grille est strict. inférieure à 3 : application de la hauteur minimale (3)\n", nomFichier);
            *h = 3;
        }

        fclose(fic);
    }
}
