#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "fichiers.h"

#define W_DEF 4
#define H_DEF 4



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
            if (n > 8)
                n = 8;
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


/**
 * @brief Retourne le nombre de lignes d'un fichier.
 */
int nb_lignes(const char *nomFichier) {
    FILE *fic = fopen(nomFichier, "r");
    char c;
    int n = 0;

    if (fic == NULL) {
        printf("Erreur lors de l'ouverture de '%s' : impossible de compter le nombre de lignes\n", nomFichier);
    } else {
        c = fgetc(fic);
        while (c != EOF) {
            if (c == '\n')
                n++;
            c = fgetc(fic);
        }
        fclose(fic);
    }
    return n;
}


/**
 * @brief Ecrit le contenu dans le fichier.
 */
void ecrire(const char *nomFichier, const char *contenu) {
    FILE *fic = fopen(nomFichier, "w");
    if (fic == NULL) {
        printf("Erreur lors de l'ouverture de '%s' : impossible d'écrire le contenu\n", nomFichier);
    } else {
        fputs(contenu, fic);
        fclose(fic);
    }
}


/**
 * @brief Ecrit en début d'un fichier le contenu passé en paramètre.
 * 
 * @param nomFichier nom du fichier où écrire le contenu
 * @param contenu contenu à écrire
 */
void ecrire_en_debut(const char *nomFichier, const char *contenu) {
    FILE *fic = fopen(nomFichier, "r");
    char **lignes;
    int n, i;

    if (fic == NULL) {
        printf("Erreur lors de l'ouverture de '%s' : impossible de lire le fichier\n", nomFichier);
    } else {
        n = nb_lignes(nomFichier) + 2;
        // On limite l'historique aux 10 dernières parties
        if (n > 20) {
            n = 19;
        }
        // On n'écrit qu'une ligne si l'historique est vide
        if (n == 2) {
            n = 1;
        }
        lignes = malloc(n * sizeof(char *));

        lignes[0] = malloc(50 * sizeof(char));
        lignes[1] = malloc(2 * sizeof(char));
        sprintf(lignes[0], "%s", contenu);
        if (n > 1) {
            lignes[1][0] = '\n';
            lignes[1][1] = '\0';
        }
        // On lit les lignes du fichier et on les mets à la suite du contenu à ajouter
        for (i = (n > 1 ? 2 : 1); i < n; i++) {
            lignes[i] = malloc(50 * sizeof(char));
            lignes[i] = fgets(lignes[i], 50, fic);
        }
        fclose(fic);

        fic = fopen(nomFichier, "w");
        if (fic == NULL) {
            printf("Erreur lors de l'ouverture de '%s' : impossible d'écrire dans le fichier\n", nomFichier);
        } else {
            // Ecriture du nouveau contenu
            for (i = 0; i < n; i++) {
                fputs(lignes[i], fic);
            }
            fclose(fic);
        }
        for (i = 0; i < n; i++) {
            free(lignes[i]);
        }
        free(lignes);
    }
}
