#include "sys.h"

// Liste de référence des catégories grammaticales reconnues
static const char *CATEGORIES_VALIDES[] = {
    "n", "n-adv", "n-suf", "n-t",
    "adj", "adj-na", "adj-no", "adj-pn", "adj-t",
    "adv", "aux", "aux-v", "conj", "exp", "int",
    "num", "pn", "pref", "suf", "prt",
    "v1", "v5", "v5aru", "v5r", "v5m", "v5k", "v5k-s",
    "v5u", "v5s", "v5b", "v5g", "v5n", "v5t", "v5z",
    "vs", "vs-s", "vi", "vt", "vk",
    nil
};

// Liste de référence des usages ou indications d'écriture
static const char *USAGES_VALIDES[] = {
    "uk", "ok", "oK", "iK", "ik", "io", "ateji", "gikun", "abbr",
    nil
};

// Liste de référence des connotations ou marques de registre
static const char *CONNOTATIONS_VALIDES[] = {
    "X", "col", "fam", "pol", "hon", "hum", "fem",
    "sl", "vulg", "vx", "arch", "obs", "pop",
    nil
};

// Supprime les espaces blancs au début et à la fin d'une chaîne
char *suppression_espace_blanc(char *chaine) {
    if (chaine == nil) {
        return nil;
    }

    while (isspace((unsigned char)*chaine)) {
        chaine++;
    }

    if (*chaine == '\0') {
        return chaine;
    }

    char *fin = chaine + strlen(chaine) - 1;

    while (fin > chaine && isspace((unsigned char)*fin)) {
        *fin = '\0';
        fin--;
    }

    return chaine;
}

// Vérifie si une chaîne ne contient que des chiffres
bool is_all_digits(const char *chaine) {
    if (chaine == nil || *chaine == '\0') {
        return false;
    }

    for (const char *p = chaine; *p != '\0'; ++p) {
        if (!isdigit((unsigned char)*p)) {
            return false;
        }
    }

    return true;
}

// Vérifie si une valeur est présente dans une liste de référence
static bool valeur_dans_reference(const char *valeur, const char *const reference[]) {
    if (valeur == nil) {
        return false;
    }

    for (size_t i = 0; reference[i] != nil; ++i) {
        if (strcmp(valeur, reference[i]) == 0) {
            return true;
        }
    }

    return false;
}

// Copie une portion de chaîne dans une nouvelle zone mémoire
static string copier_portion(const char *debut, size_t longueur) {
    string copie = malloc(longueur + 1);

    if (!copie) {
        perror("Échec d'allocation mémoire dans copier_portion");
        exit(EXIT_FAILURE);
    }

    memcpy(copie, debut, longueur);
    copie[longueur] = '\0';

    char *nettoyee = suppression_espace_blanc(copie);

    if (nettoyee != copie) {
        memmove(copie, nettoyee, strlen(nettoyee) + 1);
    }

    return copie;
}

// Classe une balise entre parenthèses dans la bonne liste
static void classer_balise(entree *entree_resultat, const char *balise, bool avant_slash) {
    if (valeur_dans_reference(balise, CATEGORIES_VALIDES)) {
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.nat.categorie_list, strdup(balise));
    } else if (valeur_dans_reference(balise, USAGES_VALIDES)) {
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.nat.usage_list, strdup(balise));
    } else if (valeur_dans_reference(balise, CONNOTATIONS_VALIDES)) {
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.nat.connotation_list, strdup(balise));
    } else if (avant_slash) {
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.nat.usage_list, strdup(balise));
    } else {
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.nat.connotation_list, strdup(balise));
    }
}

// Extrait et classe les balises entre parenthèses d'une zone de texte
static void extraire_balises(entree *entree_resultat, char *zone, bool avant_slash) {
    char *curseur = zone;

    while (curseur != nil && *curseur != '\0') {
        char *ouverture = strchr(curseur, '(');

        if (ouverture == nil) {
            break;
        }

        char *fermeture = strchr(ouverture, ')');

        if (fermeture == nil) {
            break;
        }

        string balise = copier_portion(ouverture + 1, (size_t)(fermeture - ouverture - 1));

        if (balise[0] != '\0') {
            classer_balise(entree_resultat, balise, avant_slash);
        }

        free(balise);
        curseur = fermeture + 1;
    }
}

// Ajoute le texte restant comme traduction si celui-ci n'est pas vide
static void ajouter_segment_traduction(entree *entree_resultat, char *segment) {
    char *nettoye = suppression_espace_blanc(segment);

    if (nettoye != nil && nettoye[0] != '\0') {
        ajouter_traduction(&entree_resultat->forme_fr, strdup(nettoye));
    }
}

// Traite un segment situé après le premier slash
static void traiter_segment_reste(entree *entree_resultat, char *segment) {
    char *nettoye = suppression_espace_blanc(segment);

    if (nettoye == nil || nettoye[0] == '\0') {
        return;
    }

    if (is_all_digits(nettoye)) {
        entree_resultat->id = atoi(nettoye);
        return;
    }

    while (nettoye[0] == '(') {
        char *fermeture = strchr(nettoye, ')');

        if (fermeture == nil) {
            break;
        }

        string balise = copier_portion(nettoye + 1, (size_t)(fermeture - nettoye - 1));

        if (balise[0] != '\0') {
            classer_balise(entree_resultat, balise, false);
        }

        free(balise);
        nettoye = suppression_espace_blanc(fermeture + 1);
    }

    ajouter_segment_traduction(entree_resultat, nettoye);
}

// Analyse la partie japonaise située avant le premier slash
static bool analyser_partie_japonaise(entree *entree_resultat, char *partie_japonaise) {
    char *ouverture_crochet = strchr(partie_japonaise, '[');
    char *fermeture_crochet = strchr(partie_japonaise, ']');
    char *premiere_parenthese = strchr(partie_japonaise, '(');

    if (ouverture_crochet != nil && fermeture_crochet != nil && fermeture_crochet > ouverture_crochet) {
        string forme_japonaise = copier_portion(partie_japonaise, (size_t)(ouverture_crochet - partie_japonaise));
        string furigana = copier_portion(ouverture_crochet + 1, (size_t)(fermeture_crochet - ouverture_crochet - 1));

        ajout_fin_de_liste(&entree_resultat->forme_japonaise.forme, forme_japonaise);
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.original, furigana);
        extraire_balises(entree_resultat, fermeture_crochet + 1, true);
    } else {
        size_t longueur_forme = premiere_parenthese != nil
            ? (size_t)(premiere_parenthese - partie_japonaise)
            : strlen(partie_japonaise);

        string forme_japonaise = copier_portion(partie_japonaise, longueur_forme);
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.forme, forme_japonaise);
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.original, strdup(""));

        if (premiere_parenthese != nil) {
            extraire_balises(entree_resultat, premiere_parenthese, true);
        }
    }

    return entree_resultat->forme_japonaise.forme != nil;
}

// Analyse la partie située après le premier slash
static void analyser_reste_ligne(entree *entree_resultat, char *reste) {
    char *curseur = reste;
    char *segment = nil;

    while ((segment = strsep(&curseur, "/")) != nil) {
        traiter_segment_reste(entree_resultat, segment);
    }
}

// Analyse une ligne brute et remplit une entrée structurée
bool parse_ligne(const char *ligne, entree *entree_resultat) {
    if (ligne == nil || entree_resultat == nil) {
        return false;
    }

    initialiser_entree(entree_resultat);

    char *copie = strdup(ligne);

    if (!copie) {
        perror("Échec de copie de ligne");
        exit(EXIT_FAILURE);
    }

    char *ligne_nettoyee = suppression_espace_blanc(copie);

    if (ligne_nettoyee[0] == '\0' || strncmp(ligne_nettoyee, "0000000 /", 9) == 0) {
        free(copie);
        return false;
    }

    char *premier_slash = strchr(ligne_nettoyee, '/');

    if (premier_slash == nil) {
        free(copie);
        return false;
    }

    *premier_slash = '\0';
    char *partie_japonaise = ligne_nettoyee;
    char *reste = premier_slash + 1;

    bool ok = analyser_partie_japonaise(entree_resultat, partie_japonaise);

    if (ok) {
        analyser_reste_ligne(entree_resultat, reste);
    }

    if (ok && entree_resultat->forme_japonaise.nat.categorie_list == nil) {
        ajout_fin_de_liste(&entree_resultat->forme_japonaise.nat.categorie_list, strdup("?"));
    }

    free(copie);
    return ok;
}
