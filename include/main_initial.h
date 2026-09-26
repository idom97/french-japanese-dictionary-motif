#ifndef MAIN_INITIAL_H
#define MAIN_INITIAL_H

#include "sys.h"

#ifndef TEST

size_t compter_lignes(list lignes);
entree *cons_dico(list lignes, size_t *taille_dico);
void liberer_dictionnaire(entree *dico, size_t taille_dico);

#endif

#endif