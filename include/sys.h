/*=============================================================================
	Nom         : sys.h
	Auteur      : Dominique ERIN
	Rôle        : Déclaration des prototypes des fonctions partagées
	Version     : V01
	Licence     : Réalisé dans le cadre du Cours d'ALGO (2025/2026)
	Compilation : inclus lors de la compilation du programme
	Usage       : #include "sys.h"
=============================================================================*/
#ifndef SYS_H
#define SYS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "structures.h"

// Lecture et construction des listes simples
list charger_liste(const char *nom_fichier);
list cons(void *element, const list L);
list reverse(list L);
void putlist(list L);
void ecrire(list L, const char *nom_fichier);

// Comparaison de fichiers ou de chaînes
int my_strcmp(const string s1, const string s2);
bool comparateur(const char *nom_fichier_1, const char *nom_fichier_2);

// Initialisation des structures
void initialiser_nature(Nature *nature);
void initialiser_base(base *b);
void initialiser_entree(entree *entree_resultat);

// Outils de chaînes
char *suppression_espace_blanc(char *chaine);
bool is_all_digits(const char *chaine);
void mettre_en_minuscules(char *chaine);
bool chaines_egales(const char *chaine1, const char *chaine2);

// Construction dynamique des listes
void ajout_fin_de_liste(list *tete, string valeur);
void ajouter_traduction(trads *tete, string valeur);

// Analyse des lignes du dictionnaire
bool parse_ligne(const char *ligne, entree *entree_resultat);

// Libération mémoire
void list_free(list tete);
void liberer_liste_chaines(list tete);
void liberer_traductions(trads tete);
void free_entree(entree *entree_resultat);

// Recherche et affichage
string liste_vers_chaine_virgules(list tete);
bool liste_contient_exactement(list L, const char *valeur);
bool traductions_contiennent(trads tete, const char *fragment);
void afficher_entree(entree *element);
void afficher_categories(void);
bool saisie(char *mot_temp, size_t mot_temp_taille, char *cat_temp, size_t cat_temp_taille);
void rechercher_mot(entree *dico, size_t dico_taille, const char *mot, const char *categorie);
void rechercher_cat(entree *dico, size_t dico_taille, const char *categorie, const char *nom_fichier_sortie);

// Tests
bool test_1(void);
void test_2(void);

#endif
