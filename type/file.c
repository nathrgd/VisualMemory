#include <stdio.h>
#include <malloc.h>
#include "file.h"


int file_est_vide(file *f) {
    return f == NULL;
}

file * file_vide() {
    return NULL;
}

file * enfiler(case_t *c, file *f) {
    file *nouv_file = f;
    if (file_est_vide(f)) {
        nouv_file = malloc(sizeof(file));
        nouv_file->tete = c;
        nouv_file->reste = file_vide();
    } else {
        nouv_file->reste = enfiler(c, f->reste);
    }
    return nouv_file;
}

case_t * tete(file *f) {
    return f->tete;
}

case_t * defiler(file **f) {
    file *tmp = *f;
    case_t *c = tete(tmp);
    *f = tmp->reste;
    free(tmp);
    return c;
}

void file_liberer(file *f) {
    while (!file_est_vide(f)) {
        defiler(&f);
    }
}
