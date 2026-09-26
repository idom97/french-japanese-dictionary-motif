#include "sys.h"

// Ajoute un élément en tête de liste
list cons(void *element, const list L) {
    list nouveau = malloc(sizeof(struct Doublet));

    if (!nouveau) {
        perror("Échec d'allocation mémoire pour un nœud de liste");
        exit(EXIT_FAILURE);
    }

    nouveau->car = element;
    nouveau->cdr = L;

    return nouveau;
}

// Inverse l'ordre des éléments d'une liste
list reverse(list L) {
    list resultat = nil;

    while (L != nil) {
        resultat = cons(lisp_car(L), resultat);
        L = lisp_cdr(L);
    }

    return resultat;
}

// Ajoute une valeur en fin de liste
void ajout_fin_de_liste(list *tete, string valeur) {
    if (tete == nil || valeur == nil) {
        return;
    }

    list nouveau = malloc(sizeof(struct Doublet));

    if (!nouveau) {
        perror("Échec d'allocation mémoire dans ajout_fin_de_liste");
        exit(EXIT_FAILURE);
    }

    nouveau->car = valeur;
    nouveau->cdr = nil;

    if (*tete == nil) {
        *tete = nouveau;
        return;
    }

    list courant = *tete;

    while (lisp_cdr(courant) != nil) {
        courant = lisp_cdr(courant);
    }

    courant->cdr = nouveau;
}

// Ajoute une traduction française à la liste des traductions
void ajouter_traduction(trads *tete, string valeur) {
    if (tete == nil || valeur == nil || valeur[0] == '\0') {
        free(valeur);
        return;
    }

    trads nouvelle = malloc(sizeof(struct Fr));

    if (!nouvelle) {
        perror("Échec d'allocation mémoire dans ajouter_traduction");
        exit(EXIT_FAILURE);
    }

    initialiser_base(&nouvelle->trad);
    ajout_fin_de_liste(&nouvelle->trad.forme, valeur);
    nouvelle->cdr = nil;

    if (*tete == nil) {
        *tete = nouvelle;
        return;
    }

    trads courant = *tete;

    while (traduction_suivante(courant) != nil) {
        courant = traduction_suivante(courant);
    }

    courant->cdr = nouvelle;
}

// Lit un fichier et construit une liste de lignes brutes
list charger_liste(const char *nom_fichier) {
    FILE *fp = fopen(nom_fichier, "r");

    if (!fp) {
        perror("Erreur ouverture fichier");
        return nil;
    }

    char *ligne = nil;
    size_t taille = 0;
    list resultat = nil;

    while (getline(&ligne, &taille, fp) != -1) {
        ligne[strcspn(ligne, "\r\n")] = '\0';
        resultat = cons(strdup(ligne), resultat);
    }

    free(ligne);
    fclose(fp);

    return reverse(resultat);
}

// Affiche les chaînes contenues dans une liste
void putlist(list L) {
    while (L != nil) {
        printf("%s\n", (char *)lisp_car(L));
        L = lisp_cdr(L);
    }
}

// Écrit les chaînes d'une liste dans un fichier
void ecrire(list L, const char *nom_fichier) {
    FILE *fp = fopen(nom_fichier, "w");

    if (!fp) {
        perror("Erreur lors de l'écriture dans le fichier");
        return;
    }

    while (L != nil) {
        if (lisp_car(L) != nil) {
            fprintf(fp, "%s\n", (char *)lisp_car(L));
        }

        L = lisp_cdr(L);
    }

    fclose(fp);
}

// Compare deux chaînes de caractères
int my_strcmp(const string s1, const string s2) {
    return strcmp(s1, s2);
}

// Compare deux fichiers ligne par ligne
bool comparateur(const char *nom_fichier_1, const char *nom_fichier_2) {
    FILE *fichier_1 = fopen(nom_fichier_1, "r");

    if (fichier_1 == nil) {
        perror("Erreur ouverture fichier 1");
        return false;
    }

    FILE *fichier_2 = fopen(nom_fichier_2, "r");

    if (fichier_2 == nil) {
        perror("Erreur ouverture fichier 2");
        fclose(fichier_1);
        return false;
    }

    char ligne_1[MAX_LEN];
    char ligne_2[MAX_LEN];
    bool identiques = true;
    int numero_ligne = 0;

    while (true) {
        char *lecture_1 = fgets(ligne_1, MAX_LEN, fichier_1);
        char *lecture_2 = fgets(ligne_2, MAX_LEN, fichier_2);

        if (lecture_1 == nil || lecture_2 == nil) {
            if (lecture_1 != lecture_2) {
                identiques = false;
            }
            break;
        }

        numero_ligne++;
        ligne_1[strcspn(ligne_1, "\r\n")] = '\0';
        ligne_2[strcspn(ligne_2, "\r\n")] = '\0';

        if (strcmp(ligne_1, ligne_2) != 0) {
            printf("Différence à la ligne %d.\n", numero_ligne);
            identiques = false;
            break;
        }
    }

    fclose(fichier_1);
    fclose(fichier_2);

    return identiques;
}
