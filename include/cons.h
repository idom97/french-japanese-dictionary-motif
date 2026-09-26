#ifndef CONS_H
#define CONS_H

#include "sys.h"

list cons(void *element, const list L);
list reverse(list L) ;
void ajout_fin_de_liste(list *tete, string valeur);
void ajouter_traduction(trads *tete, string valeur);
list charger_liste(const char *nom_fichier);
void putlist(list L) ;

void ecrire(list L, const char *nom_fichier);
int my_strcmp(const string s1, const string s2);
bool comparateur(const char *nom_fichier_1, const char *nom_fichier_2);

#endif
