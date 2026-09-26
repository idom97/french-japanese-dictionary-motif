#include "sys.h"
// Vérifie si un fichier existe et peut être ouvert en lecture
static bool fichier_existe(const char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "r");

    if (fichier == nil) {
        return false;
    }

    fclose(fichier);
    return true;
}

// Construit un dictionnaire à partir du fichier de test
static entree *preparer_dictionnaire(const char *nom_fichier, size_t *taille_dico) {
    list lignes = charger_liste(nom_fichier);

    if (lignes == nil) {
        fprintf(stderr, "Impossible de lire le fichier de test.\n");
        return nil;
    }

    size_t nombre_lignes = 0;

    for (list courant = lignes; courant != nil; courant = lisp_cdr(courant)) {
        nombre_lignes++;
    }

    entree *dico = calloc(nombre_lignes, sizeof(entree));

    if (!dico) {
        perror("Échec d'allocation mémoire pour le dictionnaire de test");
        liberer_liste_chaines(lignes);
        return nil;
    }

    size_t compteur = 0;

    for (list courant = lignes; courant != nil; courant = lisp_cdr(courant)) {
        const char *ligne = (const char *)lisp_car(courant);

        if (parse_ligne(ligne, &dico[compteur])) {
            compteur++;
        }
    }

    liberer_liste_chaines(lignes);
    *taille_dico = compteur;

    return dico;
}

// Libère le dictionnaire utilisé par les tests
static void liberer_dictionnaire_test(entree *dico, size_t taille_dico) {
    for (size_t i = 0; i < taille_dico; ++i) {
        free_entree(&dico[i]);
    }

    free(dico);
}

// Test de recherche par catégorie exacte
bool test_1(void) {
    size_t taille_dico = 0;
    entree *dico = preparer_dictionnaire("Dico/fj_utf8.txt", &taille_dico);

    if (dico == nil) {
        return false;
    }
    /* Vérifie que le fichier de résultat du test 1 a bien été créé */
    const char *fichier_resultat = "test_cat.txt";

    rechercher_cat(dico, taille_dico, "n", "test_cat.txt");
    
    if (fichier_existe(fichier_resultat)) {
        printf("Fichier créé : %s\n", fichier_resultat);
    } else {
        printf("Échec : le fichier %s n'a pas été créé.\n", fichier_resultat);
        liberer_dictionnaire_test(dico, taille_dico);
        return false;
    }
    
    liberer_dictionnaire_test(dico, taille_dico);

    printf("Test 1 terminé : recherche par catégorie exacte.\n");
    return true;
}

// Test de reconstruction du fichier lu en liste
void test_2(void) {
    const char *fichier_original = "Dico/fj_utf8.txt";
    const char *fichier_reconstruit = "dico_copie.txt";

    list lignes = charger_liste(fichier_original);

    if (lignes == nil) {
        fprintf(stderr, "Impossible de lire le dictionnaire original.\n");
        return;
    }

    ecrire(lignes, fichier_reconstruit);

    /* Vérifie que le fichier reconstruit a bien été créé*/
    if (fichier_existe(fichier_reconstruit)) {
        printf("Fichier créé : %s\n", fichier_reconstruit);
    } else {
        printf("Échec : le fichier %s n'a pas été créé.\n", fichier_reconstruit);
        liberer_liste_chaines(lignes);
        return;
    }

    if (comparateur(fichier_original, fichier_reconstruit)) {
        printf("Succès : le fichier reconstruit correspond au fichier original.\n");
    } else {
        printf("Échec : le fichier reconstruit diffère du fichier original.\n");
    }

    liberer_liste_chaines(lignes);
}
