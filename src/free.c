#include "sys.h"

// Libère une liste sans libérer son contenu
void list_free(list tete) {
    while (tete != nil) {
        list suivant = lisp_cdr(tete);
        free(tete);
        tete = suivant;
    }
}

// Libère une liste contenant des chaînes allouées dynamiquement
void liberer_liste_chaines(list tete) {
    while (tete != nil) {
        list suivant = lisp_cdr(tete);
        free(lisp_car(tete));
        free(tete);
        tete = suivant;
    }
}

// Libère une liste de traductions françaises
void liberer_traductions(trads tete) {
    while (tete != nil) {
        trads suivant = traduction_suivante(tete);
        liberer_liste_chaines(tete->trad.forme);
        liberer_liste_chaines(tete->trad.original);
        liberer_liste_chaines(tete->trad.nat.categorie_list);
        liberer_liste_chaines(tete->trad.nat.usage_list);
        liberer_liste_chaines(tete->trad.nat.connotation_list);
        free(tete);
        tete = suivant;
    }
}

// Libère une entrée de dictionnaire
void free_entree(entree *entree_resultat) {
    if (entree_resultat == nil) {
        return;
    }

    liberer_liste_chaines(entree_resultat->forme_japonaise.forme);
    liberer_liste_chaines(entree_resultat->forme_japonaise.original);
    liberer_liste_chaines(entree_resultat->forme_japonaise.nat.categorie_list);
    liberer_liste_chaines(entree_resultat->forme_japonaise.nat.usage_list);
    liberer_liste_chaines(entree_resultat->forme_japonaise.nat.connotation_list);
    liberer_traductions(entree_resultat->forme_fr);

    entree_resultat->forme_japonaise.forme = nil;
    entree_resultat->forme_japonaise.original = nil;
    entree_resultat->forme_japonaise.nat.categorie_list = nil;
    entree_resultat->forme_japonaise.nat.usage_list = nil;
    entree_resultat->forme_japonaise.nat.connotation_list = nil;
    entree_resultat->forme_fr = nil;
    entree_resultat->id = 0;
    entree_resultat->cdr = nil;
}
