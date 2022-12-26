#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <math.h>
#include <sys/random.h>
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
 * @brief Réinitialise l'état des cases du jeu.
 * 
 * @param tab_cases Tableau des cases du jeu
 * @param n hauteur du tableau
 * @param m largeur du tableau
 * @param texture_def texture des cases par défaut
 */
void reset_cases(case_t **tab_cases, int n, int m, SDL_Texture *texture_def) {
    SDL_Rect dstrect = {0, 0, W_CASE, H_CASE};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            dstrect.x = (X_SCREEN_CENTER - m * 0.5 * W_CASE) + j * W_CASE;
            dstrect.y = (Y_SCREEN_CENTER - n * 0.5 * H_CASE) + i * H_CASE;

            set_cliquable(&tab_cases[i][j], true);
            set_couleur(&tab_cases[i][j], BLEU);
            set_dstrect(&tab_cases[i][j], dstrect);
            set_texture(&tab_cases[i][j], texture_def);
        }
    }
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
 */
void afficher_cases(SDL_Renderer *renderer, case_t **tab_cases, int n, int m) {

    case_t c;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            c = tab_cases[i][j];
            SDL_RenderCopy(renderer, c.texture, NULL, &c.dstrect);
        }
    }
}



void afficher_vies(SDL_Renderer *renderer, int nb_vies, SDL_Texture *texture_coeur) {
    SDL_Rect dstrect = {420, 12, 36, 36};
    for (int i = 0; i < nb_vies; i++) {
        SDL_RenderCopy(renderer, texture_coeur, NULL, &dstrect);
        dstrect.x += 36;
    }
}



/**
 * @brief Sélectionne et affiche, dans l'ordre, les cases à retrouver par l'utilisateur.
 * 
 * @param tab_cases tableau de cases
 * @param n hauteur du tableau
 * @param m largeur du tableau
 * @param nb_selections nombre de cases à sélectionner
 * @param renderer renderer
 * @param texture_case_blanche texture des cases blanches 
 */
void selectionner_et_montrer_cases(jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer) {
    int i, j;
    srand(time(NULL));
    for (int k = 0; k < jeu_data->nb_cases_a_trouver; k++) {
        do {
            i = rand() % jeu_data->h_window;
            j = rand() % jeu_data->w_window;
        } while (get_couleur(jeu_data->tab_cases[i][j]) == BLANC);
        set_texture(&jeu_data->tab_cases[i][j], resources->case_blanche);
        set_couleur(&jeu_data->tab_cases[i][j], BLANC);

        jeu_data->f = enfiler(&jeu_data->tab_cases[i][j], jeu_data->f);

        SDL_RenderCopy(renderer, jeu_data->tab_cases[i][j].texture, NULL, &jeu_data->tab_cases[i][j].dstrect);
        SDL_RenderPresent(renderer);

        // Empêche l'application de ne pas répondre
        SDL_PumpEvents();

        // Pause entre chanque nouvel affichage de case
        SDL_Delay(1000);
    }
    SDL_Delay(2000);
    // Suppression d'évenements ayant eu lieu durant
    // la sélection des cases
    SDL_PumpEvents();
    SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
    SDL_FlushEvent(SDL_KEYDOWN);
}


/**
 * @brief Retourne un pointeur vers la case cliquée par l'utilisateur.
 *        Si l'utilisateur n'a pas cliqué sur une case, la fontion retourne NULL.
 * 
 * @param tab_cases tableau de cases
 * @param n nb de cases en hauteur
 * @param m nb de cases en largeur
 * @param xCursor abscisse du curseur de la souris
 * @param yCursor ordonnée du curseur de la souris
 */
case_t * recuperer_case(case_t **tab_cases, int n, int m, int xCursor, int yCursor) {
    int x_case = (X_SCREEN_CENTER - m * 0.5 * W_CASE); // Abscisse de la case en haut à gauche de la grille
    int y_case = (Y_SCREEN_CENTER - n * 0.5 * H_CASE); // Ordonnée de la case en haut à gauche de la grille

    int horiz_dist = abs(xCursor - X_SCREEN_CENTER);
    int vert_dist = abs(yCursor - Y_SCREEN_CENTER);
    int i = 0;
    int j = 0;

    // Si l'utilisateur à cliqué sur une case
    if (horiz_dist <= m * 0.5 * W_CASE && vert_dist <= n * 0.5 * H_CASE) {
        // On cherche la colonne de la case cliquée
        while (xCursor >= x_case) {
            x_case += W_CASE;
            j++;
        }
        // On cherche la ligne de la case cliquée
        while (yCursor >= y_case) {
            y_case += H_CASE;
            i++;
        }
        return &tab_cases[i-1][j-1];
    }
    return NULL;
}


char * get_texte_pour_historique(int manche, int score) {
    time_t t;
    struct tm *local;
    char *texte;
    int taille_nombres = 2;
    int n = 10;

    // Calcul du nombre de caractères nécéssaire pour afficher les nombres
    while (manche / n != 0) {
        taille_nombres++;
        n *= 10;
    }
    n = 10;
    while (score / n != 0) {
        taille_nombres++;
        n *= 10;
    }

    // Détermination de la date et de l'heure locale
    time(&t);
    local = localtime(&t);

    // 42 caractères pour le texte de base (+1 pour '\0')
    texte = malloc((taille_nombres + 43) * sizeof(char));
    sprintf(texte, "%02d/%02d/%d - %02d:%02d  :  Manche %d - Score : %d\n", local->tm_mday,
                                                                       local->tm_mon + 1,
                                                                       local->tm_year + 1900,
                                                                       local->tm_hour,
                                                                       local->tm_min,
                                                                       manche,
                                                                       score);
    return texte;
}
