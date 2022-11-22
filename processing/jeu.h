#ifndef JEU_H
#define JEU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../type/case.h"
#include "../type/file.h"


/**
 * @brief Strcture contenant les données relatives au jeu
 */
typedef struct jeu_data_s {
    case_t **tab_cases; /**< Tableau des cases du jeu */
    case_t *ptr_case_noire; /**< Pointeur vers la case noire, NULL si pas de case noire */
    int w_window; /**< Largeur de la fenêtre (en cases) */
    int h_window; /**< Hauteur de la fenêtre (en cases) */
    int score; /**< Score du joueur */
    int nb_cases_a_trouver; /**< Nombre de cases à trouver */
    int vies; /**< Nombre de vies */
    int program_launched; /**< Indique si le programme doit rester lancé */
    int new_round; /**< Indique si une nouvelle manche doit être lancée */
    char message_resultat[50]; /**< Message à afficher en fin de manche */
    file f; /**< File contenant les cases à trouver */
} jeu_data_t;


/**
 * @brief Structure contenant les ressources utilisées dans le jeu.
 */
typedef struct resources_s {
    SDL_Texture *case_bleue; /**< Texture de la case bleue */
    SDL_Texture *case_noire; /**< Texture de la case noire */
    SDL_Texture *case_blanche; /**< Texture de la case blanche */
    SDL_Texture *texte_vies; /**< Texture du texte affichant le nombre de vies */
    SDL_Texture *texte_resultat; /**< Texture du texte affichant le résultat de la manche */
    SDL_Texture *fond; /**< Texture du fond */
    TTF_Font *font; /**< Police d'écriture */
} resources_t;


void init_jeu_data(jeu_data_t *jeu_data, int w_window, int h_window);
void init_resources(resources_t *resources, SDL_Renderer *renderer);
void init_new_round(jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer);
void gerer_evenements(SDL_Event *event, jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer);
void update_graphics(jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer);
void update_historique(jeu_data_t *jeu_data);
void free_jeu_data(jeu_data_t jeu_data);
void free_resources(resources_t resources);

#endif
