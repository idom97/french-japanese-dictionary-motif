#include "main_initial.h"

#ifndef TEST

// Compte le nombre de lignes brutes présentes dans une liste
size_t compter_lignes(list lignes) {
    size_t compteur = 0;

    while (lignes != nil) {
        compteur++;
        lignes = lisp_cdr(lignes);
    }

    return compteur;
}

// Construit le tableau d'entrées à partir de la liste des lignes brutes
entree *cons_dico(list lignes, size_t *taille_dico) {
    size_t nombre_lignes = compter_lignes(lignes);
    entree *dico = calloc(nombre_lignes, sizeof(entree));

    if (!dico) {
        perror("Échec d'allocation mémoire pour le dictionnaire");
        exit(EXIT_FAILURE);
    }

    size_t compteur = 0;

    for (list courant = lignes; courant != nil; courant = lisp_cdr(courant)) {
        const char *ligne = (const char *)lisp_car(courant);

        if (parse_ligne(ligne, &dico[compteur])) {
            compteur++;
        }
    }

    *taille_dico = compteur;
    return dico;
}

// Libère le dictionnaire structuré
void liberer_dictionnaire(entree *dico, size_t taille_dico) {
    if (dico == nil) {
        return;
    }

    for (size_t i = 0; i < taille_dico; ++i) {
        free_entree(&dico[i]);
    }

    free(dico);
}

#endif