#include "sys.h"

// Initialise les champs de la structure Nature
void initialiser_nature(Nature *nature) {
    if (nature == nil) {
        return;
    }

    nature->categorie_list = nil;
    nature->usage_list = nil;
    nature->connotation_list = nil;
}

// Initialise les champs de la structure Base
void initialiser_base(base *b) {
    if (b == nil) {
        return;
    }

    b->forme = nil;
    b->original = nil;
    initialiser_nature(&b->nat);
}

// Initialise les champs d'une entrée de dictionnaire
void initialiser_entree(entree *entree_resultat) {
    if (entree_resultat == nil) {
        return;
    }

    initialiser_base(&entree_resultat->forme_japonaise);
    entree_resultat->forme_fr = nil;
    entree_resultat->id = 0;
    entree_resultat->cdr = nil;
}
