#ifndef RECHERCHE_H
#define RECHERCHE_H

#include "sys.h"

void mettre_en_minuscules(char *chaine);
bool chaines_egales(const char *chaine1, const char *chaine2);
bool chaine_contient_fragment(const char *chaine, const char *fragment);
string liste_vers_chaine_virgules(list tete);
bool liste_contient_exactement(list tete, const char *valeur);
bool traductions_contiennent(trads tete, const char *fragment);
string traductions_vers_chaine(trads tete);
void afficher_entree(entree *element);
//void rechercher_mot(entree *dico, size_t dico_taille, const char *mot, const char *categorie);

void rechercher_cat(entree *dico, size_t dico_taille, const char *categorie, const char *nom_fichier_sortie);

#endif