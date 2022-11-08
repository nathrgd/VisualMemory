#include <stdio.h>
#include <stdlib.h>

#include "processing/processing.h"
#include "fichiers.h"
#include "fonctions_SDL.h"
#include "type/case.h"
#include "type/file.h"


int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    SDL_Texture *texture_case_bleue, *texture_case_noire, *texture_case_blanche, *texture_texte_vies, *texture_fond;
    TTF_Font *font;
    SDL_Rect dstrect_texte_vies;
    SDL_bool program_launched = SDL_TRUE, new_round = SDL_TRUE;

    case_t **tab_cases;
    case_t *ptr_case, *ptr_case_noire = NULL;

    file f = file_vide();

    char message_texte_vies[100];

    int nb_cases_a_trouver, vies, w, h;


    lire_dimensions("grille.txt", &w, &h);
    
    init_SDL(&window, &renderer, w*W_CASE, (h + 1)*H_CASE);


    // Chargement du texte indiquant le nombre de vies restantes
    font = TTF_OpenFont("police/arial.ttf", 20);
    texture_texte_vies = charger_texte("Vies : 3", renderer, font, (SDL_Color){255, 255, 255, 255});
    dstrect_texte_vies = (SDL_Rect){W_CASE/6, H_CASE/12, W_CASE, H_CASE/2};

    // Chargement des textures
    texture_fond = charger_image("img/fond.bmp", renderer);
    texture_case_bleue = charger_image("img/case_bleue.bmp", renderer);
    texture_case_noire = charger_image("img/case_noire.bmp", renderer);
    texture_case_blanche = charger_image("img/case_blanche.bmp", renderer);


    tab_cases = create_tab_cases(h, w);
    reset_cases(tab_cases, h, w, texture_case_bleue);


    int a, b;
    nb_cases_a_trouver = 3;
    while (program_launched) {
        SDL_RenderClear(renderer);

        // Application du fond
        SDL_RenderCopy(renderer, texture_fond, NULL, NULL);

        if (new_round) {
            file_liberer(f);
            f = file_vide();

            a = 0;
            b = 0;
            vies = nb_cases_a_trouver;
            reset_cases(tab_cases, h, w, texture_case_bleue);
            render_copy(renderer, tab_cases, h, w);
            selectionner_et_montrer_cases(tab_cases, h, w, nb_cases_a_trouver, renderer, texture_case_blanche, &f);
            reset_cases(tab_cases, h, w, texture_case_bleue);
            new_round = SDL_FALSE;

            // Suppression d'évenements ayant eu lieu durant
            // la sélection des cases
            SDL_PumpEvents();
            SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
            SDL_FlushEvent(SDL_KEYDOWN);
        }

        render_copy(renderer, tab_cases, h, w);

        // Affichage du texte indiquant le nombre de vies restantes
        sprintf(message_texte_vies, "Vies : %d", vies);
        update_message_text(message_texte_vies, &texture_texte_vies, renderer, font, (SDL_Color){255, 255, 255, 255});
        SDL_RenderCopy(renderer, texture_texte_vies, NULL, &dstrect_texte_vies);

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    program_launched = SDL_FALSE;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            program_launched = SDL_FALSE;
                            break;
                        case SDLK_ESCAPE:
                            new_round = SDL_TRUE;
                            break;
                        case SDLK_n:
                            set_couleur(&tab_cases[0][a < w ? a : a-1], BLANC);
                            set_cliquable(&tab_cases[0][a < w ? a : a-1], false);
                            set_texture(&tab_cases[0][a < w ? a : a-1], texture_case_blanche);
                            if (a < w) a++;
                            break;
                        case SDLK_b:
                            set_couleur(&tab_cases[1][b < w ? b : b-1], NOIR);
                            set_cliquable(&tab_cases[1][b < w ? b : b-1], false);
                            set_texture(&tab_cases[1][b < w ? b : b-1], texture_case_noire);
                            if (b < w) b++;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    case SDL_BUTTON_LEFT:
                        ptr_case = recuperer_case(tab_cases, h, w, event.button.x, event.button.y);

                        if (ptr_case != NULL && est_cliquable(*ptr_case)) {
                            // La dernière case noire redevient bleue lorsqu'une autre case est cliquée
                            if (ptr_case_noire != NULL) {
                                set_couleur(ptr_case_noire, BLEU);
                                set_texture(ptr_case_noire, texture_case_bleue);
                                set_cliquable(ptr_case_noire, true);
                                ptr_case_noire = NULL;
                            }
                            if (cases_correspondent(ptr_case, tete(f))) {
                                // La case cliquée est correcte
                                set_couleur(ptr_case, BLANC);
                                set_texture(ptr_case, texture_case_blanche);
                                set_cliquable(ptr_case, false);
                                defiler(&f);
                            } else {
                                set_couleur(ptr_case, NOIR);
                                set_texture(ptr_case, texture_case_noire);
                                set_cliquable(ptr_case, false);
                                ptr_case_noire = ptr_case;
                                vies -= 1;
                            }
                        }
                        break;
                    break;
                default:
                    break;
            }

            // On stoppe la partie si le joueur a trouvé toutes les cases
            if (file_est_vide(f)) {
                render_copy(renderer, tab_cases, h, w);
                SDL_RenderPresent(renderer);
                printf("Gagné !\n");
                new_round = SDL_TRUE;
                SDL_Delay(2000);

                nb_cases_a_trouver++;
                if (nb_cases_a_trouver > h * w) {
                    printf("Vous avez ateint le niveau maximal !\n");
                    program_launched = SDL_FALSE;
                }

                // Suppression d'évenements dans la file pour éviter que l'OS croit que le jeu ne répond pas
                SDL_PumpEvents();
                SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
                break;
            }


            // On stoppe la partie si le joueur n'a plus de vies
            if (vies == 0) {
                // MAJ du texte indiquant le nombre de vies restantes
                sprintf(message_texte_vies, "Vies : %d", vies);
                update_message_text(message_texte_vies, &texture_texte_vies, renderer, font, (SDL_Color){255, 255, 255, 255});
                SDL_RenderCopy(renderer, texture_texte_vies, NULL, &dstrect_texte_vies);

                render_copy(renderer, tab_cases, h, w);
                SDL_RenderPresent(renderer);
                printf("Perdu !\n");
                new_round = SDL_TRUE;
                SDL_Delay(2000);

                nb_cases_a_trouver = 3;

                // Suppression d'évenements dans la file pour éviter que l'OS croit que le jeu ne répond pas
                SDL_PumpEvents();
                SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
                break;
            }
        }
    }

    free_tab_cases(tab_cases, h);
    file_liberer(f);


    SDL_DestroyTexture(texture_fond);
    SDL_DestroyTexture(texture_texte_vies);
    SDL_DestroyTexture(texture_case_bleue);
    SDL_DestroyTexture(texture_case_noire);
    SDL_DestroyTexture(texture_case_blanche);
                nb_cases_a_trouver = 2;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();

    return EXIT_SUCCESS;
}
