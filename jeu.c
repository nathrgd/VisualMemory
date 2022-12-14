#include <stdio.h>
#include <string.h>
#include "jeu.h"
#include "processing.h"
#include "type/file.h"
#include "fichiers.h"
#include "fonctions_SDL.h"


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
    jeu_data->score = 0;
    jeu_data->nb_cases_a_trouver = 3;
    jeu_data->vies = 3;
    jeu_data->program_launched = 1;
    jeu_data->new_round = 1;
    jeu_data->message_resultat[0] = '\0';
    jeu_data->f = file_vide();
}


/**
 * @brief Initialise les ressources du jeu.
 */
void init_resources(resources_t *resources, SDL_Renderer *renderer) {
    resources->case_bleue = charger_image("img/case_bleue.bmp", renderer);
    resources->case_noire = charger_image("img/case_noire.bmp", renderer);
    resources->case_blanche = charger_image("img/case_blanche.bmp", renderer);
    resources->coeur = charger_image("img/coeur.bmp", renderer);
    resources->fond = charger_image("img/fond.bmp", renderer);
    resources->font = TTF_OpenFont("police/arial.ttf", 20);
    resources->texte_manche = NULL;
    resources->texte_vies = charger_texte("Vies : ", renderer, resources->font, (SDL_Color){255, 255, 255});
    resources->texte_resultat = NULL;
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
    // Le nombre maximum de vies est de 10
    if (jeu_data->vies >10) {
        jeu_data->vies = 10;
    }

    jeu_data->message_resultat[0] = '\0';

    reset_cases(jeu_data->tab_cases, jeu_data->h_window, jeu_data->w_window, resources->case_bleue);
    update_graphics(jeu_data, resources, renderer);

    selectionner_et_montrer_cases(jeu_data, resources, renderer);
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
                            jeu_data->score++;
                        } else {
                            set_couleur(ptr_case, NOIR);
                            set_texture(ptr_case, resources->case_noire);
                            set_cliquable(ptr_case, false);
                            jeu_data->ptr_case_noire = ptr_case;
                            jeu_data->vies -= 1;
                            jeu_data->score -= jeu_data->score > 0 ? 1 : 0;
                        }
                    }
                    break;
                break;
            default:
                break;
        }

        // On stoppe la partie si le joueur a trouvé toutes les cases
        if (file_est_vide(jeu_data->f)) {
            sprintf(jeu_data->message_resultat, "Bravo !");
            update_graphics(jeu_data, resources, renderer);
            jeu_data->nb_cases_a_trouver++;
            if (jeu_data->nb_cases_a_trouver > jeu_data->h_window * jeu_data->w_window) {
                sprintf(jeu_data->message_resultat, "Gagne !");
                jeu_data->nb_cases_a_trouver--; // Désincrémentation pour déterminer manche pour historique
                jeu_data->program_launched = 0;
            }
            jeu_data->new_round = 1;
            SDL_Delay(2000);

            break;
        }


        // On stoppe la partie si le joueur n'a plus de vies
        if (jeu_data->vies == 0) {
            sprintf(jeu_data->message_resultat, "Perdu !");
            update_graphics(jeu_data, resources, renderer);
            SDL_Delay(3000);

            jeu_data->program_launched = 0;
            break;
        }

        jeu_data->message_resultat[0] = '\0';
    }
}


/**
 * @brief Met à jour les graphismes du jeu.
 * 
 */
void update_graphics(jeu_data_t *jeu_data, resources_t *resources, SDL_Renderer *renderer) {
    char message_texte_manche[50];
    SDL_Color blanc = {255, 255, 255};
    SDL_Rect dstrect_texte_manche = {20, 10, 150, 40};
    SDL_Rect dstrect_texte_vies = {320, 10, 100, 40};
    SDL_Rect dstrect_texte_resultat = {360, (650 - (325 - 0.5 * jeu_data->h_window * H_CASE) * 0.5) - 20, 100, 40};


    // Adaptation de la taille du texte
    if (strcmp(jeu_data->message_resultat, "Manche gagne !") == 0) {
        dstrect_texte_resultat.x = 310;
        dstrect_texte_resultat.y -= 5;
        dstrect_texte_resultat.w = 180;
        dstrect_texte_resultat.h = 50;
    }

    // Application du fond
    SDL_RenderCopy(renderer, resources->fond, NULL, NULL);


    // MAJ du texte indiquant la manche actuelle
    sprintf(message_texte_manche, "Manche : %d", jeu_data->nb_cases_a_trouver - 2);
    update_message_text(message_texte_manche, &resources->texte_manche, renderer, resources->font, blanc);
    SDL_RenderCopy(renderer, resources->texte_manche, NULL, &dstrect_texte_manche);

    // MAJ du texte indiquant le nombre de vies restantes
    afficher_vies(renderer, jeu_data->vies, resources->coeur);
    SDL_RenderCopy(renderer, resources->texte_vies, NULL, &dstrect_texte_vies);

    // MAJ du texte indiquant le résultat de la partie
    if (jeu_data->message_resultat[0] != '\0') {
        update_message_text(jeu_data->message_resultat, &resources->texte_resultat, renderer, resources->font, blanc);
        SDL_RenderCopy(renderer, resources->texte_resultat, NULL, &dstrect_texte_resultat);
    }

    afficher_cases(renderer, jeu_data->tab_cases, jeu_data->h_window, jeu_data->w_window);
    SDL_RenderPresent(renderer);
    SDL_Delay(10); // Pause pour éviter un bug d'affichage
}



void update_historique(jeu_data_t *jeu_data) {
    char * texte = get_texte_pour_historique(jeu_data->nb_cases_a_trouver - 2, jeu_data->score);
    ecrire_en_debut("historique.txt", texte);
    free(texte);
}


void update_derniere_grille(jeu_data_t *jeu_data) {
    char *texte = malloc((jeu_data->h_window * (jeu_data->w_window + 1) + 1) * sizeof(char));
    int cpt = 0, continuer;
    char c;

    // Modélisation de la grille par une chaîne de caractères
    // Initialisation de texte
    for (int i = 0; i < jeu_data->h_window * (jeu_data->w_window + 1); i++) {
        texte[i] = '0';
        if (cpt == jeu_data->w_window) {
            texte[i] = '\n';
            cpt = 0;
        } else {
            cpt++;
        }
    }
    texte[jeu_data->h_window * (jeu_data->w_window + 1)] = '\0';

    for (int k = 0; k < jeu_data->nb_cases_a_trouver; k++) {
        case_t *ptr_case = defiler(&jeu_data->f);
        continuer = 1;
        for (int i = 0; i < jeu_data->h_window && continuer; i++) {
            for (int j = 0; j < jeu_data->w_window && continuer; j++) {
                if (cases_correspondent(&jeu_data->tab_cases[i][j], ptr_case)) {
                    c = k + '1';
                    if (c > '9') {
                        c = '+';
                    }
                    texte[i * (jeu_data->w_window + 1) + j] = c;
                    cpt--;
                    continuer = 0;
                }
            }
        }
        // On réenfile le pointeur pour que la file soit la même qu'au début de la fonction
        jeu_data->f = enfiler(ptr_case, jeu_data->f);
    }

    // Ecriture du contenu de la grille dans derniere_grille.txt
    ecrire("derniere_grille.txt", texte);
    free(texte);
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
    SDL_DestroyTexture(resources.coeur);
    SDL_DestroyTexture(resources.texte_vies);
    SDL_DestroyTexture(resources.texte_manche);
    SDL_DestroyTexture(resources.texte_resultat);
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
