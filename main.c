#include <stdio.h>
#include <stdlib.h>

#include "processing/processing.h"
#include "fichiers.h"
#include "fonctions_SDL.h"
#include "processing/jeu.h"



/**
 * @brief Libère la mémoire allouée pour le jeu et quitte SDL.
 * 
 * @param window Fenêtre du jeu
 * @param renderer Renderer du jeu
 * @param jeu_data Données du jeu
 * @param resources Ressources du jeu
 */
void clean(SDL_Window *window, SDL_Renderer *renderer, jeu_data_t jeu_data, resources_t resources) {
    free_jeu_data(jeu_data);
    free_resources(resources);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}



int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    jeu_data_t jeu_data;
    resources_t resources;

    SDL_Event event;

    int w, h;


    lire_dimensions("grille.txt", &w, &h);
    
    init_SDL(&window, &renderer, W_SCREEN, H_SCREEN);

    init_jeu_data(&jeu_data, w, h);
    init_resources(&resources, renderer);

    reset_cases(jeu_data.tab_cases, h, w, resources.case_bleue);


    while (jeu_data.program_launched) {
        SDL_RenderClear(renderer);

        if (jeu_data.new_round) {
            init_new_round(&jeu_data, &resources, renderer);
            update_derniere_grille(&jeu_data);
        }

        update_graphics(&jeu_data, &resources, renderer);

        gerer_evenements(&event, &jeu_data, &resources, renderer);
    }
    update_historique(&jeu_data);
            
    clean(window, renderer, jeu_data, resources);

    return EXIT_SUCCESS;
}
