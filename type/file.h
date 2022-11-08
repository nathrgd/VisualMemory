#ifndef FILE_H
#define FILE_H

#include "case.h"

typedef struct file_s {
    case_t *tete;
    struct file_s *reste;
} *file;


int file_est_vide(file f);
file file_vide(void);
file enfiler(case_t *c, file f);
case_t *defiler(file *f);
case_t *tete(file f);
void file_liberer(file f);


#endif