/*=============================================================================
	Nom         : structures.h
	Auteur      : Dominique ERIN
	Rôle        : Déclaration des structures de données du dictionnaire
	Version     : V01
	Licence     : Réalisé dans le cadre du Cours d'ALGO (2025/2026)
	Compilation : inclus lors de la compilation du programme
	Usage       : #include "structures.h"
=============================================================================*/
#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_LEN 1024

// Valeur utilisée pour représenter la fin d'une liste
#define nil NULL

typedef char* string;

// Nœud fondamental d'une liste de type Lisp
typedef struct Doublet {
    void *car;
    struct Doublet *cdr;
} *list;

// Primitives de liste conservées pour respecter le formalisme Lisp
#define lisp_car(x) ((x)->car)
#define lisp_cdr(x) ((x)->cdr)

// Attributs de nature : catégories grammaticales, usages et connotations
typedef struct Nature {
    list categorie_list;
    list usage_list;
    list connotation_list;
} Nature;

// Structure commune aux formes manipulées par le programme
typedef struct Base {
    list forme;
    list original;
    Nature nat;
} base;

// Structure Fr - Structure pour la forme française
typedef struct Fr {
    base trad;
    struct Fr *cdr;
} *trads;

// Structure Entree - Une seule entrée du dictionnaire
typedef struct Entree {
    base forme_japonaise;
    trads forme_fr;
    int id;
    struct Entree *cdr;
} entree;

// Accès aux champs d'une Entree*
#define base_jp(e)          ((e)->forme_japonaise)
#define liste_trad_fr(e)    ((e)->forme_fr)
#define entree_id(e)        ((e)->id)
#define entree_suivante(e)  ((e)->cdr)

// Accès aux champs d'une Base
#define base_forme(b)         (b).forme
#define base_original(b)      (b).original
#define base_categories(b)    (b).nat.categorie_list
#define base_usages(b)        (b).nat.usage_list
#define base_connotations(b)  (b).nat.connotation_list

// Accès à la base d'une traduction française
#define base_trad_fr(ptr_fr)  ((ptr_fr)->trad)

// Accès à la forme japonaise principale
#define forme(ptr_entree) ((string)lisp_car(base_forme(base_jp(ptr_entree))))

// Accès au furigana
#define original(ptr_entree) ((string)lisp_car(base_original(base_jp(ptr_entree))))

// Accès à la liste des traductions françaises
#define traductions(ptr_entree) liste_trad_fr(ptr_entree)

// Accès aux catégories grammaticales de l'entrée japonaise
#define cat(ptr_entree) base_categories(base_jp(ptr_entree))

// Accès aux usages de l'entrée japonaise
#define usages(ptr_entree) base_usages(base_jp(ptr_entree))

// Accès aux connotations de l'entrée japonaise
#define connotations_jp(ptr_entree) base_connotations(base_jp(ptr_entree))

// Accès à la traduction principale d'une cellule Fr*
#define traduction_forme(ptr_fr) ((string)lisp_car(base_forme(base_trad_fr(ptr_fr))))

// Accès au complément éventuel d'une cellule Fr*
#define traduction_original(ptr_fr) ((string)lisp_car(base_original(base_trad_fr(ptr_fr))))

// Accès aux connotations associées à une traduction française
#define connotations_trad(ptr_fr) base_connotations(base_trad_fr(ptr_fr))

// Accès à la traduction française suivante
#define traduction_suivante(ptr_fr) ((ptr_fr)->cdr)

#endif
