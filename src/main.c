#include "sys.h"

#ifndef TEST

// Compte le nombre de lignes brutes présentes dans une liste
static size_t compter_lignes(list lignes) {
    size_t compteur = 0;

    while (lignes != nil) {
        compteur++;
        lignes = lisp_cdr(lignes);
    }

    return compteur;
}

// Construit le tableau d'entrées à partir de la liste des lignes brutes
static entree *construire_dictionnaire(list lignes, size_t *taille_dico) {
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
static void liberer_dictionnaire(entree *dico, size_t taille_dico) {
    if (dico == nil) {
        return;
    }

    for (size_t i = 0; i < taille_dico; ++i) {
        free_entree(&dico[i]);
    }

    free(dico);
}

#endif

int main(void) {
#ifdef TEST
    printf("TEST 1...\n");
    test_1();
    printf("TEST 2...\n");
    test_2();
    printf("Fin des tests.\n");
    return EXIT_SUCCESS;
#else
    char mot[MAX_LEN];
    char categorie[MAX_LEN];
    const char *nom_fichier = "Dico/fj_utf8.txt";

    list lignes = charger_liste(nom_fichier);

    if (lignes == nil) {
        fprintf(stderr, "Impossible de lire le fichier dictionnaire.\n");
        return EXIT_FAILURE;
    }

    size_t taille_dico = 0;
    entree *dico = construire_dictionnaire(lignes, &taille_dico);
    liberer_liste_chaines(lignes);

    if (taille_dico == 0) {
        fprintf(stderr, "Aucune entrée exploitable n'a été construite.\n");
        free(dico);
        return EXIT_FAILURE;
    }

    if (!saisie(mot, MAX_LEN, categorie, MAX_LEN)) {
        fprintf(stderr, "Erreur de saisie.\n");
        liberer_dictionnaire(dico, taille_dico);
        return EXIT_FAILURE;
    }

    rechercher_mot(dico, taille_dico, mot, categorie);
    liberer_dictionnaire(dico, taille_dico);

    return EXIT_SUCCESS;
#endif
}
