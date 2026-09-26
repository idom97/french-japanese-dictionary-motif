# Diko - Dictionnaire bilingue français-japonais avec une interface Motif

Ce projet vise à implémenter un dictionnaire bilingue français-japonais en C/C++, avec une interface graphique réalisée avec Motif.

Lorsque le programme démarre, l’utilisateur peut saisir un mot français ou japonais avec sa catégorie grammaticale. Le résultat est affiché dans une zone graphique d’historique.

Contrairement à Python, le langage C ne fournit pas de type dictionnaire intégré comparable au type `dict` de Python. Pour cette raison, le projet utilise des structures C personnalisées et des listes chaînées inspirées de Lisp pour stocker et accéder aux entrées du dictionnaire.

Le fichier dictionnaire utilisé dans ce projet provient de FJDICT, créé par J.-M. Desperrier et d’autres contributeurs en 2002. Sa structure générale est la suivante :

```text
(((kanji) ([furigana]) (category)) ((French translation) ((usage)(additional French translation))) (identifier))
```

## Déclaration des listes et des structures

Après avoir identifié la structure du fichier dictionnaire, nous avons créé les quatre structures C suivantes. Ces structures regroupent les éléments qui composent une entrée du dictionnaire.

1. La structure `Nature` contient la catégorie grammaticale d’un mot, son usage ou sa connotation, et son identifiant :

```C
typedef struct Nature {
    // Attributs de nature : catégories grammaticales et usages
    list categorie_list; // liste de chaînes de caractères
    list usage_list;     // liste de chaînes de caractères
    int code;            // identifiant de la ligne
} Nature;
```

2. La structure `Base` contient la forme principale d’un mot. Pour un mot japonais, cette forme principale peut être la forme en kanji, et la forme originale peut être le furigana. Pour une traduction française, la même structure peut aussi servir à stocker la forme française et ses informations complémentaires. La structure contient aussi les attributs de `Nature` :

```C
// Structure de Base - contient les listes pour les chaînes de caractères forme/original
typedef struct Base {
    list forme;    // Une liste où le 'car' est la chaîne de caractères (Kanji, mot français)
    list original; // Une liste où le 'car' est la chaîne de caractères (Furigana, traduction complémentaire)
    Nature nat;    // Attributs de nature
} base;
```

3. La structure `Fr` représente une traduction française d’un mot japonais. Elle contient la traduction française, ses usages, et les traductions complémentaires possibles. Elle contient aussi un pointeur vers la traduction française suivante :

```C
// Structure Fr (traduction française) - Structure pour la forme française
typedef struct Fr {
    base traduction; // Contient la forme, l'original et les usages pour une traduction
    struct Fr *cdr;  // Pointeur vers la traduction suivante
} *traductions;
```

4. La structure `Entree` représente une entrée du dictionnaire. Elle contient la forme japonaise du mot, la liste des traductions françaises, et l’identifiant attaché à l’entrée :

```C
// Structure Entree - une seule entrée de dictionnaire
typedef struct Entree {
    base forme_japonaise; // Informations sur le mot japonais
    traductions forme_fr; // Une liste de Fr* pour plusieurs traductions françaises
    int id;               // Identifiant de l'entrée
} entree;
```

## Accès aux listes chaînées

Nous accédons aux listes chaînées avec des macros de type Lisp inspirées de `car` et `cdr`.

En Lisp, `car` donne accès au premier élément d’une liste. Dans ce projet, la macro `lisp_car` donne accès au contenu du nœud courant de la liste :

```C
#define lisp_car(x) ((x)->car) // Accède au contenu du nœud de liste
```

La fonction `cdr` donne accès au reste de la liste. Dans ce projet, la macro `lisp_cdr` donne accès au nœud suivant de la liste :

```C
#define lisp_cdr(x) ((x)->cdr) // Accède au nœud suivant de la liste
```

Avant d’accéder aux éléments d’une `entree`, nous définissons des macros de type Lisp qui rendent le code plus lisible.

1. Accès à la base japonaise :

```C
#define jp_base(e) ((e)->forme_japonaise) // Accède à la Base japonaise
```

2. Accès à la liste des traductions françaises :

```C
#define liste_trad_fr(e) ((e)->forme_fr) // Accède à la liste de traductions françaises
```

3. Accès à l’identifiant de l’entrée :

```C
#define entree_id(e) ((e)->id) // Accède à l'identifiant de l'entrée
```

Nous déclarons ensuite les macros de type Lisp suivantes pour accéder aux champs internes des structures.

1. Accès à une structure `base`, en utilisant `b` comme nom de la variable de base :

```C
#define base_form(b) (b).forme
#define base_original(b) (b).original
#define base_categories(b) (b).nat.categorie_list
#define base_usages(b) (b).nat.usage_list
```

2. Accès direct à une traduction française avec un pointeur `Fr*` :

```C
// Accès aux champs d'une Fr*
// Par exemple, fr_ptr peut être obtenu avec lisp_car sur une liste de traductions françaises
#define fr_translation_base(fr_ptr) ((fr_ptr)->traduction)
```

3. Accès au mot japonais, par exemple la forme en kanji. Ici, `e_ptr` est un pointeur vers une structure `Entree` :

```C
// Accès à un mot japonais (Kanji)
// 'forme' est une list dont le car contient la chaîne de caractères.
#define form(e_ptr) ((string)lisp_car(base_form(jp_base(e_ptr))))
```

4. Accès à la prononciation d’un mot japonais, par exemple le furigana. Ici encore, `e_ptr` est un pointeur vers une structure `Entree` :

```C
// Accès à l'original (Furigana)
#define original(e_ptr) ((string)lisp_car(base_original(jp_base(e_ptr))))
```

5. Accès à la liste des traductions françaises d’un mot japonais :

```C
// Accès aux traductions françaises
// Retourne une liste de traductions.
#define traductions(e_ptr) liste_trad_fr(e_ptr)
```

6. Accès aux catégories grammaticales d’un mot japonais :

```C
// Accès aux catégories grammaticales
#define cat(e_ptr) base_categories(jp_base(e_ptr))

#define nil NULL
```

Ce projet a été réalisé par **Dominique ERIN**, sous la direction du Professeur **Larbi BOUBCHIR** à **l’Université Paris 8**.

---

## 📚 REMARQUES, CRÉDITS ET DONNÉES

Ce projet a été réalisé dans le cadre du cours **Algorithmique et Structures de données**, rédigé par le Professeur **Gilles Bernard**.

Certaines parties du projet proviennent d’un code initial Mini-Lisp / Motif écrit par **Gilles Bernard**. Les commentaires et mentions d’origine ont été conservés dans les fichiers sources.

Le fichier dictionnaire utilisé par le programme provient de **FJDICT** :

```text
FJDICT 20DEC02 V00-001-PR3
Dictionnaire francais-japonais version preliminaire 3
Copyright J.-M. Desperrier + autres - 2002
```

Le contenu du dictionnaire n’est pas la propriété de l’auteur de ce projet. Toute redistribution du fichier dictionnaire doit conserver sa mention de copyright originale.

En-tête original du fichier dictionnaire :

```text
0000000 /FJDICT 20DEC02 V00-001-PR3/Dictionnaire francais-japonais version preliminaire 3/Copyright J.-M. Desperrier + autres - 2002/
```

Le travail réalisé dans ce projet porte sur :

- le chargement du fichier dictionnaire ;
- le découpage des lignes du dictionnaire ;
- la construction d’une structure interne de recherche ;
- la recherche par mot et catégorie grammaticale ;
- l’ajout d’une interface graphique avec Motif ;
- l’ajout d’une zone d’historique des traductions ;
- l’ajout de tests.

---

## ⚙️ COMPILATION DU PROGRAMME

Pour compiler le dictionnaire graphique, utilisez la commande suivante :

```bash
$ make
```

![Compilation](docs/screenshots/CE1.png "Compilation")

---

## 🚀 LANCEMENT DU PROGRAMME

Pour exécuter l’application graphique :

```bash
$ ./Diko
```

![Lancement](docs/screenshots/CE2.png "Lancement")

---

## 🧪 EXEMPLE D’ERREUR DE SAISIE

Si l’utilisateur saisit seulement un mot sans catégorie grammaticale :

```text
ameliorer
```

le programme affiche un message d’erreur indiquant le format attendu.

![Erreur de saisie](docs/screenshots/CE3.png "Erreur de saisie")

---

## 🧭 AFFICHAGE DE L’AIDE DES CATÉGORIES

Pour afficher la liste des catégories grammaticales disponibles, l’utilisateur peut saisir :

```text
H
```

ou saisir `H` après un mot :

```text
ameliorer H
```

![Aide des catégories](docs/screenshots/CE4.png "Aide des catégories")

---

## 🔎 RECHERCHE D’UNE TRADUCTION FRANÇAIS-JAPONAIS

Exemple avec le verbe :

```text
ameliorer v1
```

![Recherche ameliorer](docs/screenshots/CE5.png "Recherche ameliorer")

---

## 🇯🇵 RECHERCHE AVEC UNE ENTRÉE JAPONAISE

L’interface permet aussi à l’utilisateur de saisir une entrée japonaise qui existe dans le dictionnaire.

Exemple :

```text
磨き上げる
```

Sans catégorie grammaticale, le programme affiche une erreur de format.

![Erreur de saisie japonaise](docs/screenshots/CE6.png "Erreur de saisie japonaise")

Avec la catégorie grammaticale :

```text
磨き上げる v1
```

le programme trouve l’entrée correspondante.

![Recherche japonaise](docs/screenshots/CE7.png "Recherche japonaise")

---

## 🔎 AUTRE EXEMPLE DE RECHERCHE

Exemple avec le verbe :

```text
manger v1
```

![Recherche manger](docs/screenshots/CE8.png "Recherche manger")

---

## 🧪 TESTS DU PROGRAMME

Le projet contient une cible de test dans le `Makefile`.

Pour lancer les tests :

```bash
$ make test
```

![Tests](docs/screenshots/CE9.png "Tests")

Les tests vérifient :

- la recherche par catégorie grammaticale ;
- la création du fichier `test_cat.txt` ;
- la reconstruction du fichier dictionnaire dans `dico_copie.txt` ;
- la comparaison entre le fichier original et le fichier reconstruit.

Exemple de sortie :

```text
TEST 1...
Fichier créé : test_cat.txt
Test 1 terminé : recherche par catégorie exacte.
TEST 2...
Fichier créé : dico_copie.txt
Succès : le fichier reconstruit correspond au fichier original.
Fin des tests.
Tests terminés.
```

---

## 📄 DESCRIPTION DES PRINCIPAUX FICHIERS

<dl>
  <dt><code>src/dico_gui.cpp</code></dt>
  <dd>Programme principal de la version graphique. Il crée l’interface Motif, charge le dictionnaire, récupère la saisie utilisateur et affiche les résultats dans la zone d’historique.</dd>

  <dt><code>include/dico_gui.h</code></dt>
  <dd>Header principal de la version graphique. Il regroupe les inclusions nécessaires au programme Motif.</dd>

  <dt><code>src/main_initial.c</code></dt>
  <dd>Contient des fonctions provenant de la version initiale du dictionnaire.</dd>

  <dt><code>src/parse.c</code></dt>
  <dd>Analyse les lignes du fichier dictionnaire afin d’extraire les mots, les lectures, les catégories et les traductions.</dd>

  <dt><code>src/recherche.c</code></dt>
  <dd>Contient les fonctions de recherche utilisées dans le dictionnaire.</dd>

  <dt><code>src/interaction.c</code></dt>
  <dd>Contient l’affichage des catégories grammaticales disponibles.</dd>

  <dt><code>src/test.c</code></dt>
  <dd>Contient les fonctions de test du programme.</dd>

  <dt><code>Dico/fj_utf8.txt</code></dt>
  <dd>Fichier dictionnaire français-japonais utilisé par le programme.</dd>

  <dt><code>Makefile</code></dt>
  <dd>Automatise la compilation du programme principal et des tests.</dd>
</dl>

---

## 📦 DÉPENDANCES

Le projet nécessite :

- `gcc` ;
- `g++` ;
- `make` ;
- les bibliothèques Motif / OpenMotif ;
- les bibliothèques X11.

Sous GNU/Linux, les dépendances peuvent être installées avec :

```bash
$ sudo apt install build-essential libmotif-dev libx11-dev
```

---

## 🧹 NETTOYAGE DES FICHIERS COMPILÉS

Pour supprimer les fichiers objets, l’exécutable principal, l’exécutable de test, et les fichiers générés par les tests :

```bash
$ make clean
```

---

## 👤 AUTEUR

Projet réalisé par :

```text
Dominique ERIN
```

Interface graphique réalisée avec :

```text
Motif
```

---
