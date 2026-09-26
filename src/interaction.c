#include "sys.h"

// Vide le reste de la ligne après une lecture avec scanf
static void vider_reste_ligne(void) {
    int caractere;

    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        ;
    }
}

// Affiche le menu d'aide des catégories grammaticales
void afficher_categories(void) {
    printf("\n");
    printf("======================================================================\n");
    printf("CATÉGORIES GRAMMATICALES DISPONIBLES\n");
    printf("======================================================================\n");
    printf("\nNoms et formes nominales :\n");
    printf("  n, n-adv, n-suf, n-t, num, pn\n");
    printf("\nAdjectifs :\n");
    printf("  adj, adj-na, adj-no, adj-pn, adj-t\n");
    printf("\nAdverbes, particules et expressions :\n");
    printf("  adv, conj, exp, int, pref, suf, prt\n");
    printf("\nVerbes :\n");
    printf("  v1, v5, v5aru, v5r, v5m, v5k, v5k-s\n");
    printf("  v5u, v5s, v5b, v5g, v5n, v5t, v5z, vs, vs-s, vi, vt, vk\n");
    printf("\nAuxiliaires :\n");
    printf("  aux, aux-v\n");
    printf("======================================================================\n\n");
}

// Demande une saisie rudimentaire avec scanf et propose l'aide pour les catégories
bool saisie(char *mot_temp, size_t mot_temp_taille, char *cat_temp, size_t cat_temp_taille) {
    if (mot_temp == nil || cat_temp == nil || mot_temp_taille == 0 || cat_temp_taille == 0) {
        return false;
    }

    char format_mot[32];
    char format_cat[32];

    snprintf(format_mot, sizeof(format_mot), "%%%zus", mot_temp_taille - 1);
    snprintf(format_cat, sizeof(format_cat), "%%%zus", cat_temp_taille - 1);

    printf("Dico fr-jap\n");

    while (true) {
        printf("Veuillez saisir un mot à traduire en français ou en japonais : ");

        if (scanf(format_mot, mot_temp) != 1) {
            printf("Erreur lors de la saisie du mot.\n");
            vider_reste_ligne();
            return false;
        }

        vider_reste_ligne();

        if (mot_temp[0] == '\0') {
            printf("Le mot ne peut pas être vide. Veuillez réessayer.\n");
            continue;
        }

        while (true) {
            printf("Précisez la catégorie grammaticale du mot à traduire ");
            printf("(obligatoire, H pour afficher l'aide) : ");

            if (scanf(format_cat, cat_temp) != 1) {
                printf("Erreur lors de la saisie de la catégorie.\n");
                vider_reste_ligne();
                return false;
            }

            vider_reste_ligne();

            if (chaines_egales(cat_temp, "H") || chaines_egales(cat_temp, "h")) {
                afficher_categories();
                continue;
            }

            if (cat_temp[0] == '\0') {
                printf("La catégorie grammaticale est obligatoire.\n");
                continue;
            }

            mettre_en_minuscules(cat_temp);
            return true;
        }
    }
}
