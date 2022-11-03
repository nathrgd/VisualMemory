#include <stdio.h>
#include <malloc.h>
#include "processing.h"


/**
 * @brief Créer un tableau de cases de dimensions nxm
 * 
 * @param n hauteur du tableau
 * @param m largeur du tableau
 * @return référence vers le tableau de cases
 */
case_t **create_tab_cases(int n, int m) {
    case_t **tab = malloc(n * sizeof(case_t *));
    for (int i = 0; i < n; i++) {
        tab[i] = malloc(m * sizeof(case_t));
    }
    return tab;
}



/**
 * @brief Libère la mémoire allouée pour un tableau de cases
 * 
 * @param tab tableau de cases
 * @param n hauteur du tableau
 */
void free_tab_cases(case_t **tab, int n) {
    for (int i = 0; i < n; i++) {
        free(tab[i]);
    }
    free(tab);
}


/**
 * @brief Copie dans le renderer les textures correspondantes aux cases du tableau.
 * 
 * @param renderer renderer
 * @param tab_cases tableau de cases
 * @param n hauteur du tableau
 * @param m largeur du tableau
 * @param texture_case_bleue texture d'une case bleue
 * @param texture_case_noire texture d'une case noire
 * @param texture_case_blanche texture d'une case blanche
 */
void render_copy(SDL_Renderer *renderer, case_t **tab_cases, int n, int m) {

    case_t c;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            c = tab_cases[i][j];
            SDL_RenderCopy(renderer, c.texture, NULL, &c.dstrect);
        }
    }
}
