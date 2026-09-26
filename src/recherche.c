/*=============================================================================
	Nom         : recherche.c
	Auteur      : Dominique ERIN
	Rôle        : Recherche des traductions dans le dictionnaire
	Version     : V01
	Licence     : Réalisé dans le cadre du Cours d'ALGO (2025/2026)
	Compilation : make
	Usage       : utilisé par le programme principal avec interface graphique
=============================================================================*/
#include "sys.h"

// Convertit uniquement les majuscules ASCII en minuscules
void mettre_en_minuscules(char *chaine) {
    if (chaine == nil) {
        return;
    }

    for (size_t i = 0; chaine[i] != '\0'; i++) {
        if (chaine[i] >= 'A' && chaine[i] <= 'Z') {
            chaine[i] = chaine[i] + ('a' - 'A');
        }
    }
}

// Vérifie si deux chaînes sont identiques
bool chaines_egales(const char *chaine1, const char *chaine2) {
    if (chaine1 == nil || chaine2 == nil) {
        return false;
    }

    return strcmp(chaine1, chaine2) == 0;
}

// Vérifie si une chaîne contient un fragment
static bool chaine_contient_fragment(const char *chaine, const char *fragment) {
    if (chaine == nil || fragment == nil || fragment[0] == '\0') {
        return false;
    }

    char copie_chaine[MAX_LEN];
    char copie_fragment[MAX_LEN];

    strncpy(copie_chaine, chaine, sizeof(copie_chaine) - 1);
    copie_chaine[sizeof(copie_chaine) - 1] = '\0';

    strncpy(copie_fragment, fragment, sizeof(copie_fragment) - 1);
    copie_fragment[sizeof(copie_fragment) - 1] = '\0';

    mettre_en_minuscules(copie_chaine);
    mettre_en_minuscules(copie_fragment);

    return strstr(copie_chaine, copie_fragment) != nil;
}

// Convertit une liste de chaînes en une chaîne séparée par des virgules
string liste_vers_chaine_virgules(list tete) {
    size_t longueur = 1;

    for (list courant = tete; courant != nil; courant = lisp_cdr(courant)) {
        string valeur = (string)lisp_car(courant);
        longueur += valeur != nil ? strlen(valeur) + 2 : 0;
    }

    string resultat = calloc(longueur, sizeof(char));

    if (!resultat) {
        perror("Échec d'allocation mémoire dans liste_vers_chaine_virgules");
        exit(EXIT_FAILURE);
    }

    for (list courant = tete; courant != nil; courant = lisp_cdr(courant)) {
        string valeur = (string)lisp_car(courant);

        if (valeur == nil) {
            continue;
        }

        if (resultat[0] != '\0') {
            strcat(resultat, ", ");
        }

        strcat(resultat, valeur);
    }

    return resultat;
}

// Vérifie si une liste contient exactement une valeur
bool liste_contient_exactement(list tete, const char *valeur) {
    if (valeur == nil) {
        return false;
    }

    for (list courant = tete; courant != nil; courant = lisp_cdr(courant)) {
        string element = (string)lisp_car(courant);

        if (element != nil && chaines_egales(element, valeur)) {
            return true;
        }
    }

    return false;
}

// Vérifie si les traductions françaises contiennent le fragment recherché
bool traductions_contiennent(trads tete, const char *fragment) {
    if (fragment == nil || fragment[0] == '\0') {
        return false;
    }

    for (trads courant = tete; courant != nil; courant = traduction_suivante(courant)) {
        if (chaine_contient_fragment(traduction_forme(courant), fragment)) {
            return true;
        }
    }

    return false;
}

// Construit une chaîne avec toutes les traductions françaises
static string traductions_vers_chaine(trads tete) {
    size_t longueur = 1;

    for (trads courant = tete; courant != nil; courant = traduction_suivante(courant)) {
        string valeur = traduction_forme(courant);
        longueur += valeur != nil ? strlen(valeur) + 2 : 0;
    }

    string resultat = calloc(longueur, sizeof(char));

    if (!resultat) {
        perror("Échec d'allocation mémoire dans traductions_vers_chaine");
        exit(EXIT_FAILURE);
    }

    for (trads courant = tete; courant != nil; courant = traduction_suivante(courant)) {
        string valeur = traduction_forme(courant);

        if (valeur == nil) {
            continue;
        }

        if (resultat[0] != '\0') {
            strcat(resultat, ", ");
        }

        strcat(resultat, valeur);
    }

    return resultat;
}

// Affiche une entrée sous une forme structurée
void afficher_entree(entree *element) {
    string categories = liste_vers_chaine_virgules(cat(element));
    string liste_usages = liste_vers_chaine_virgules(usages(element));
    string liste_connotations = liste_vers_chaine_virgules(connotations_jp(element));
    string liste_traductions = traductions_vers_chaine(traductions(element));

    printf("%s ", forme(element) != nil ? forme(element) : "");

    if (original(element) != nil && original(element)[0] != '\0') {
        printf("[%s] ", original(element));
    }

    if (categories[0] != '\0') {
        printf("[catégorie : %s] ", categories);
    }

    if (liste_usages[0] != '\0') {
        printf("[usage : %s] ", liste_usages);
    }

    if (liste_connotations[0] != '\0') {
        printf("[connotation : %s] ", liste_connotations);
    }

    printf("%s\n", liste_traductions);

    free(categories);
    free(liste_usages);
    free(liste_connotations);
    free(liste_traductions);
}

// Recherche un mot et une catégorie exacte dans le dictionnaire
void rechercher_mot(entree *dico, size_t dico_taille, const char *mot, const char *categorie) {
    if (dico == nil || mot == nil || categorie == nil) {
        return;
    }

    char mot_recherche[MAX_LEN];
    char categorie_recherche[MAX_LEN];
    bool trouve = false;

    strncpy(mot_recherche, mot, sizeof(mot_recherche) - 1);
    mot_recherche[sizeof(mot_recherche) - 1] = '\0';

    strncpy(categorie_recherche, categorie, sizeof(categorie_recherche) - 1);
    categorie_recherche[sizeof(categorie_recherche) - 1] = '\0';

    mettre_en_minuscules(mot_recherche);
    mettre_en_minuscules(categorie_recherche);

    for (size_t i = 0; i < dico_taille; i++) {
        entree *element = &dico[i];

        bool correspondance_mot = false;

        if (chaine_contient_fragment(forme(element), mot_recherche)) {
            correspondance_mot = true;
        }

        if (chaine_contient_fragment(original(element), mot_recherche)) {
            correspondance_mot = true;
        }

        if (traductions_contiennent(traductions(element), mot_recherche)) {
            correspondance_mot = true;
        }

        bool correspondance_categorie = liste_contient_exactement(cat(element), categorie_recherche);

        if (correspondance_mot && correspondance_categorie) {
            afficher_entree(element);
            trouve = true;
        }
    }

    if (!trouve) {
        printf("Aucune entrée trouvée pour ce mot avec cette catégorie.\n");
    }
}

// Recherche des entrées par catégorie et écrit le résultat dans un fichier
void rechercher_cat(entree *dico, size_t dico_taille, const char *categorie, const char *nom_fichier_sortie) {
    FILE *fp = fopen(nom_fichier_sortie, "w");

    if (fp == nil) {
        perror("Erreur ouverture fichier de sortie");
        return;
    }

    for (size_t i = 0; i < dico_taille; ++i) {
        entree *element = &dico[i];

        if (liste_contient_exactement(cat(element), categorie)) {
            fprintf(fp, "%s\n", forme(element) != nil ? forme(element) : "");
        }
    }

    fclose(fp);
}
