#include <stdio.h>
#include "jeu.h"
#include "processing.h"
#include "../fonctions_SDL.h"


/**
 * @brief Initialise les données du jeu.
 * 
 * @param jeu_data Structure contenant les données du jeu.
 * @param w Largeur de la fenêtre
 * @param h Hauteur de la fenêtre
 */
void init_jeu_data(jeu_data_t *jeu_data, int w, int h) {
    jeu_data->tab_cases = create_tab_cases(h, w);
    jeu_data->ptr_case_noire = NULL;
    jeu_data->w_window = w;
    jeu_data->h_window = h;
    jeu_data->nb_cases_a_trouver = 3;
    jeu_data->vies = 3;
    jeu_data->program_launched = 1;
    jeu_data->new_round = 1;
    jeu_data->f = file_vide();
}


/**
 * @brief Initialise les ressources du jeu.
 */
void init_resources(resources_t *resources, SDL_Renderer *renderer) {
    resources->case_bleue = charger_image("img/case_bleue.bmp", renderer);
    resources->case_noire = charger_image("img/case_noire.bmp", renderer);
    resources->case_blanche = charger_image("img/case_blanche.bmp", renderer);
    resources->fond = charger_image("img/fond.bmp", renderer);
    resources->font = TTF_OpenFont("police/arial.ttf", 20);
    resources->texte_vies = NULL;
}


/**
 * @brief Initialise une nouvelle manche.
 * 
 * @param jeu_data Données du jeu
 * @param resources Ressources du jeu
 * @param renderer Renderer
 */
void init_new_round(jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer) {
    file_liberer(jeu_data->f);
    jeu_data->f = file_vide();

    jeu_data->vies = jeu_data->nb_cases_a_trouver / 2;

    reset_cases(jeu_data->tab_cases, jeu_data->h_window, jeu_data->w_window, resources->case_bleue);
    update_graphics(jeu_data, resources, renderer);

    selectionner_et_montrer_cases(jeu_data->tab_cases, jeu_data->h_window, jeu_data->w_window, jeu_data->nb_cases_a_trouver, renderer, resources->case_blanche, &jeu_data->f);
    reset_cases(jeu_data->tab_cases, jeu_data->h_window, jeu_data->w_window, resources->case_bleue);

    jeu_data->new_round = 0;
}


/**
 * @brief Gère les événements SDL.
 */
void gerer_evenements(SDL_Event *event, jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer) {
    case_t *ptr_case;

    while (SDL_PollEvent(event)) {
        switch (event->type) {
            case SDL_QUIT:
                jeu_data->program_launched = 0;
                break;
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_q:
                        jeu_data->program_launched = 0;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                case SDL_BUTTON_LEFT:
                    ptr_case = recuperer_case(jeu_data->tab_cases, jeu_data->h_window, jeu_data->w_window, event->button.x, event->button.y);

                    if (ptr_case != NULL && est_cliquable(*ptr_case)) {
                        // La dernière case noire redevient bleue lorsqu'une autre case est cliquée
                        if (jeu_data->ptr_case_noire != NULL) {
                            set_couleur(jeu_data->ptr_case_noire, BLEU);
                            set_texture(jeu_data->ptr_case_noire, resources->case_bleue);
                            set_cliquable(jeu_data->ptr_case_noire, true);
                            jeu_data->ptr_case_noire = NULL;
                        }
                        if (cases_correspondent(ptr_case, tete(jeu_data->f))) {
                            // La case cliquée est correcte
                            set_couleur(ptr_case, BLANC);
                            set_texture(ptr_case, resources->case_blanche);
                            set_cliquable(ptr_case, false);
                            defiler(&jeu_data->f);
                        } else {
                            set_couleur(ptr_case, NOIR);
                            set_texture(ptr_case, resources->case_noire);
                            set_cliquable(ptr_case, false);
                            jeu_data->ptr_case_noire = ptr_case;
                            jeu_data->vies -= 1;
                        }
                    }
                    break;
                break;
            default:
                break;
        }

        // On stoppe la partie si le joueur a trouvé toutes les cases
        if (file_est_vide(jeu_data->f)) {
            update_graphics(jeu_data, resources, renderer);
            printf("Gagné !\n");
            jeu_data->new_round = 1;
            SDL_Delay(2000);

            jeu_data->nb_cases_a_trouver++;
            if (jeu_data->nb_cases_a_trouver > jeu_data->h_window * jeu_data->w_window) {
                printf("Vous avez ateint le niveau maximal !\n");
                jeu_data->program_launched = 0;
            }
            break;
        }


        // On stoppe la partie si le joueur n'a plus de vies
        if (jeu_data->vies == 0) {
            update_graphics(jeu_data, resources, renderer);
            printf("Perdu !\n");
            jeu_data->new_round = 1;
            SDL_Delay(2000);

            jeu_data->nb_cases_a_trouver = 3;
            break;
        }
    }
}


/**
 * @brief Met à jour les graphismes du jeu.
 * 
 */
void update_graphics(jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer) {
    char message_texte_vies[50];
    SDL_Rect dstrect_texte_vies = {W_CASE/6, H_CASE/12, W_CASE, H_CASE/2};

    // Application du fond
    SDL_RenderCopy(renderer, resources->fond, NULL, NULL);

    // MAJ du texte indiquant le nombre de vies restantes
    sprintf(message_texte_vies, "Vies : %d", jeu_data->vies);
    update_message_text(message_texte_vies, &resources->texte_vies, renderer, resources->font, (SDL_Color){255, 255, 255, 255});
    SDL_RenderCopy(renderer, resources->texte_vies, NULL, &dstrect_texte_vies);

    render_copy(renderer, jeu_data->tab_cases, jeu_data->h_window, jeu_data->w_window);
    SDL_RenderPresent(renderer);
    SDL_Delay(10); // Pause pour éviter un bug d'affichage
}


/**
 * @brief Libère la mémoire allouée pour les ressources du jeu.
 * 
 * @param resources ressources du jeu
 */
void free_resources(resources_t resources) {
    SDL_DestroyTexture(resources.case_bleue);
    SDL_DestroyTexture(resources.case_blanche);
    SDL_DestroyTexture(resources.case_noire);
    SDL_DestroyTexture(resources.texte_vies);
    SDL_DestroyTexture(resources.fond);
    TTF_CloseFont(resources.font);
}



/**
 * @brief Libère la mémoire allouée pour les données du jeu.
 * 
 * @param jeu_data données du jeu
 */
void free_jeu_data(jeu_data_t jeu_data) {
    free_tab_cases(jeu_data.tab_cases, jeu_data.h_window);
    file_liberer(jeu_data.f);
}