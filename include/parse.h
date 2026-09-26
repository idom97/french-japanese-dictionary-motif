#ifndef PARSE_H
#define PARSE_H

#include "sys.h"

char *suppression_espace_blanc(char *chaine);
bool is_all_digits(const char *chaine);
bool valeur_dans_reference(const char *valeur, const char *const reference[]);
string copier_portion(const char *debut, size_t longueur);
void classer_balise(entree *entree_resultat, const char *balise, bool avant_slash);
void extraire_balises(entree *entree_resultat, char *zone, bool avant_slash);
void ajouter_segment_traduction(entree *entree_resultat, char *segment);
void traiter_segment_reste(entree *entree_resultat, char *segment);
bool analyser_partie_japonaise(entree *entree_resultat, char *partie_japonaise);
void analyser_reste_ligne(entree *entree_resultat, char *reste);
bool parse_ligne(const char *ligne, entree *entree_resultat);

#endif