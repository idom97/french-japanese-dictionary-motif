/*=============================================================================
Nom         : dico_gui.cpp
Auteur      : Dominique ERIN
Rôle        : Programme principal de traduction avec interface graphique
Version     : V01
Licence     : Réalisé dans le cadre du Cours d'ALGO (2025/2026)
Compilation : make
Usage       : ./Diko_GUI
=============================================================================*/
/* Mini-Lisp à compléter
   Ecrit par Gilles Bernard, février 2009
   Licence GPL


C'est du C avec quelques commodités offertes par C++ ; il faut installer les librairies openmotif.

pour compiler, utiliser make dans le répertoire des fichiers sources

*/
#include "dico_gui.h"
#include <locale.h> /* Pour éviter que le compilateur cherche sur le disque on créé le fichier locale_locale.h dans le dossier include */
/* AJOUT INTERFACE STRUCTURÉE
   Les headers suivants sont nécessaires pour passer d'une console graphique
   unique à une interface séparant :
   - un champ de recherche ;
   - un bouton Traduire ;
   - une zone d'historique avec défilement.
*/
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/LabelG.h>
#include <Xm/TextF.h>
#include <Xm/PushB.h>

void gui_print(Widget text, const char *message);

void rechercher_mot_gui(Widget text,
                        entree *dico,
                        size_t taille_dico,
                        const char *mot,
                        const char *categorie);

XmFontList creer_fontlist_diko(Widget widget);
void traduire_call(Widget W, XtPointer app_data, XtPointer call_data);
void afficher_categories_gui(Widget text);

/* ========================================== GLOBALES ============================================*/

static const char *DIKO_FONT =
    "-*-fixed-medium-r-normal--44-*-*-*-*-*-iso10646-1,"
    "-jis-fixed-medium-r-normal--24-170-100-100-c-240-jisx0208.1983-0";

// Causes de throw et messages correspondants
// Pour Done (catch), read et read_list (throw)
//ConstantString Throw[5] = {"liste inachevée", "cdr manquant (après un point)", "cons inachevé (après un point)", "trop d'éléments dans le cdr (après un point)", "trop de parenthèses fermantes"} ;

// pour contrôler le curseur dans l'ancienne interface de type console graphique
XmTextPosition Start_pos = 0 ;

// AJOUT 1
entree *dico = NULL ;
size_t taille_dico = 0 ;

/* AJOUT INTERFACE STRUCTURÉE
   Ces widgets doivent être globaux car la fonction traduire_call est appelée
   après l'initialisation de l'interface, lorsque l'utilisateur clique sur
   le bouton Traduire ou appuie sur Entrée dans le champ de recherche.
*/
static Widget champ_recherche = NULL ;
static Widget historique = NULL ;

/*
objet nil ;
objet Current_Input ;
objet Current_Output ;
flatlist GcAllobjects = NULL ;
objet * Oblist ;
positive GcNumber = 0 ;
positive Obindex = 0 ; // destiné à disparaître avec le hachage
objet GcTemp ;
*/

/* ============================================= MAIN ======================================================*/

int main(int argc, String argv[])
{
    setlocale(LC_ALL, "");
    XtSetLanguageProc(NULL, NULL, NULL);

    XtAppContext interface ;

    // créer la fenêtre principale et ses sous-parties motif
    /* VERSION D'ORIGINE LISP
    Widget top_widget = XtVaAppInitialize(&interface, "Interlisp", NULL, 0, &argc, argv, NULL, NULL) ;
    Widget saisie = initMotifWidgets(top_widget, interface) ;
    */

    Widget top_widget = XtVaAppInitialize(&interface, "MyDico - Fr vers Jp", NULL, 0, &argc, argv, NULL, NULL) ;

    XtVaSetValues(top_widget,
        XmNwidth, 1200,
        XmNheight, 800,
        NULL
    );

    Widget saisie = initMotifWidgets(top_widget, interface) ;
    (void)saisie;

    /* ANCIEN AJOUT LOCAL À NE PLUS UTILISER
       Ces déclarations étaient locales à main et n'étaient donc pas visibles
       depuis Done ou traduire_call. Elles ont été déplacées dans les globales.

    static Widget champ_recherche;
    static Widget historique;
    */

    /* ANCIEN TEST DE POLICE À NE PLUS UTILISER
       Cette version reposait sur une seule police simple et pouvait casser
       l'affichage des caractères japonais.

    XFontStruct *font = XLoadQueryFont(
        XtDisplay(saisie),
        "-misc-fixed-medium-r-normal--20-200-75-75-c-100-iso10646-1"
    );

    if (font == NULL) {
        font = XLoadQueryFont(XtDisplay(saisie), "fixed");
    }

    XmFontList fontlist = XmFontListCreate(font, XmSTRING_DEFAULT_CHARSET);

    XtVaSetValues(saisie,
        XmNfontList, fontlist,
        NULL
    );

    XmFontListFree(fontlist);
    */

    // saisie est déjà géré dans initMotifWidgets :
    //XtManageChild(saisie) ;

    // initialiser le dictionnaire

    // initialiser Lisp
    //initLisp() ;

    /* ANCIENNE INITIALISATION DU PROMPT LISP
    XmTextSetString(saisie, (String) "? ") ;
    Start_pos = 2 ;
    XmTextSetCursorPosition(saisie, Start_pos) ;
    */

    /* ANCIENNE POSITION DU XtRealizeWidget
       Dans la version actuelle, on charge d'abord le dictionnaire. Si le fichier
       dictionnaire est absent ou inexploitable, on quitte avant d'afficher la fenêtre.

    XtRealizeWidget(top_widget) ;
    XtAppMainLoop(interface) ;
    */

    // Ajout 2 en provenance du main initial
    const char *nom_fichier = "Dico/fj_utf8.txt";

    list lignes = charger_liste(nom_fichier);

    if (lignes == nil) {
        fprintf(stderr, "Impossible de lire le fichier dictionnaire.\n");
        return EXIT_FAILURE;
    }

    dico = cons_dico(lignes, &taille_dico);
    liberer_liste_chaines(lignes);

    if (taille_dico == 0) {
        fprintf(stderr, "Aucune entrée exploitable n'a été construite.\n");
        free(dico);
        return EXIT_FAILURE;
    }

    /* ANCIENNE INITIALISATION DE LA CONSOLE GRAPHIQUE
       L'interface actuelle n'utilise plus de prompt dico>. La saisie se fait
       dans champ_recherche et l'affichage se fait dans historique.

    XmTextSetString(saisie, (String) "dico> ") ;
    Start_pos = 6 ;
    XmTextSetCursorPosition(saisie, Start_pos) ;
    */

    if (historique != NULL) {
        gui_print(historique,
            "Format attendu : <mot> <categorie>. Le mot doit etre saisi sans accent.\n"
            "Exemple : ameliorer v1\n\n"
            "Pour afficher la liste des categories prises en compte, tapez H seul ou tapez H apres le mot.\n"
            "Exemple : bonjour H\n\n"
        );
    }

    XtRealizeWidget(top_widget) ;
    XtAppMainLoop(interface) ;

    return 0 ;
}

/*============================================ FONCTIONS WIDGET ==========================================*/

// action lors d'un <Enter> : lance le read_eval_print
// boucler tant que le read n'a pas épuisé la ligne
// line contient toute la ligne
// S contient la partie de la ligne restant à traiter
void Done(Widget text, XEvent * event, String * params, Cardinal * n)
{
    /* VERSION D'ORIGINE LISP
    String line ;
    if (not (* (line = XmTextGetString(text) + Start_pos))) return ;
    Current_Input = alloc_objet(sizeof(InputString)) ;
    objet_type(Current_Input) = ISTRING ;
    Current_Output = alloc_objet(sizeof(Widget *)) ;
    objet_type(Current_Output) = OWIDGET ;
    Owidget(Current_Output) = text ;
    try {Widget_read_eval_print(line) ;}
    catch(int reason) {fprintf(stdout, "Attention : %s\n", Throw[reason]) ; return ;} ;
    */

    (void)text;
    (void)event;
    (void)params;
    (void)n;

    traduire_call(NULL, NULL, NULL);
}

/* C'est du lisp
void widget_print(objet output, String S)
{ XmTextInsert(Owidget(output), XmTextGetLastPosition(Owidget(output)), S) ;
}

void widget_newpos(objet output)
{ Start_pos = XmTextGetLastPosition(Owidget(output)) ; }
*/

/* =============================================== FONCTIONS ==============================================*/

// initialisation des widgets
/* VERSION D'ORIGINE 
Widget initMotifWidgets(Widget top_wid, XtAppContext app) 
{ Widget main_window = XtVaCreateManagedWidget("main_window", xmMainWindowWidgetClass, top_wid, NULL) ;

  Widget menu_bar = XmCreateMenuBar(main_window, (String) "main_list", NULL, 0) ;
  XtManageChild(menu_bar) ;

  // créer le bouton quit avec son callback
  Widget quit = XtVaCreateManagedWidget("Quit", xmCascadeButtonWidgetClass, menu_bar, NULL);
  XtAddCallback(quit, XmNactivateCallback, quit_call, NULL) ;

  // créer la zone de saisie et ses ressources 
  Arg args[10];
  int n = initSetArg(args) ;

  Widget saisie = XmCreateScrolledText(main_window, (String) "saisie", args, n) ;
  XtManageChild(saisie) ;
  
  // gérer le retour-charriot
  static XtActionsRec actions[] = {{(String) "Done", Done}} ;
  XtAppAddActions(app, actions, XtNumber(actions));

  static ConstantString traduction = "<Key>Return: Done()" ;
  XtTranslations mytranslations = XtParseTranslationTable(traduction) ;
  XtOverrideTranslations(saisie, mytranslations) ;

  // gérer le curseur
  XtAddCallback(saisie, XmNmodifyVerifyCallback, control_insert, NULL) ;

  return saisie ;
}
*/

// NOUVELLE VERSION : interface structurée en champ de saisie + bouton + historique
Widget initMotifWidgets(Widget top_wid, XtAppContext app)
{
    (void)app;

    Widget main_window = XtVaCreateManagedWidget(
        "main_window",
        xmMainWindowWidgetClass,
        top_wid,
        NULL
    );

    XmFontList fontlist_diko = creer_fontlist_diko(main_window);

    Widget form_principal = XmCreateForm(main_window, (String) "form_principal", NULL, 0);

    /* Cadre supérieur : saisie de la commande */
    Widget frame_recherche = XtVaCreateManagedWidget(
        "frame_recherche",
        xmFrameWidgetClass,
        form_principal,
        XmNtopAttachment, XmATTACH_FORM,
        XmNleftAttachment, XmATTACH_FORM,
        XmNrightAttachment, XmATTACH_FORM,
        XmNtopOffset, 12,
        XmNleftOffset, 12,
        XmNrightOffset, 12,
        NULL
    );

    XtVaCreateManagedWidget(
        "Entrer un mot pour le traduire",
        xmLabelGadgetClass,
        frame_recherche,
        XmNchildType, XmFRAME_TITLE_CHILD,
        XmNfontList, fontlist_diko,
        NULL
    );

    Widget form_recherche = XmCreateForm(frame_recherche, (String) "form_recherche", NULL, 0);

    champ_recherche = XmCreateTextField(form_recherche, (String) "champ_recherche", NULL, 0);

    XtVaSetValues(
        champ_recherche,
        XmNtopAttachment, XmATTACH_FORM,
        XmNbottomAttachment, XmATTACH_FORM,
        XmNleftAttachment, XmATTACH_FORM,
        XmNrightAttachment, XmATTACH_POSITION,
        XmNrightPosition, 68,
        XmNtopOffset, 8,
        XmNbottomOffset, 8,
        XmNleftOffset, 8,
        XmNrightOffset, 8,
        XmNfontList, fontlist_diko,
        NULL
    );

    XtManageChild(champ_recherche);

    Widget bouton_traduire = XtVaCreateManagedWidget(
        "Traduire",
        xmPushButtonWidgetClass,
        form_recherche,
        XmNtopAttachment, XmATTACH_FORM,
        XmNbottomAttachment, XmATTACH_FORM,
        XmNleftAttachment, XmATTACH_POSITION,
        XmNleftPosition, 70,
        XmNrightAttachment, XmATTACH_FORM,
        XmNtopOffset, 8,
        XmNbottomOffset, 8,
        XmNleftOffset, 8,
        XmNrightOffset, 8,
        XmNfontList, fontlist_diko,
        NULL
    );

    XtAddCallback(bouton_traduire, XmNactivateCallback, traduire_call, NULL);
    XtAddCallback(champ_recherche, XmNactivateCallback, traduire_call, NULL);

    XtManageChild(form_recherche);
    
    /* Champ Légende précisant infos de auteur : moi */
    Widget label_auteur = XtVaCreateManagedWidget(
        "Dictionnaire bilingue FR-JP par Dominique ERIN",
        xmLabelGadgetClass,
        form_principal,
        XmNbottomAttachment, XmATTACH_FORM,
        XmNleftAttachment, XmATTACH_FORM,
        XmNrightAttachment, XmATTACH_FORM,
        XmNbottomOffset, 8,
        XmNleftOffset, 12,
        XmNrightOffset, 12,
        XmNalignment, XmALIGNMENT_CENTER,
        XmNfontList, fontlist_diko,
        NULL
    );

    /* Cadre inférieur : historique des traductions */
    Widget frame_historique = XtVaCreateManagedWidget(
        "frame_historique",
        xmFrameWidgetClass,
        form_principal,
        XmNtopAttachment, XmATTACH_WIDGET,
        XmNtopWidget, frame_recherche,
        //XmNbottomAttachment, XmATTACH_FORM,
        // Adapté au champ légende
        XmNbottomAttachment, XmATTACH_WIDGET,
        XmNbottomWidget, label_auteur,
        XmNleftAttachment, XmATTACH_FORM,
        XmNrightAttachment, XmATTACH_FORM,
        XmNtopOffset, 16,
        XmNbottomOffset, 12,
        XmNleftOffset, 12,
        XmNrightOffset, 12,
        NULL
    );

    XtVaCreateManagedWidget(
        "Historique des traductions",
        xmLabelGadgetClass,
        frame_historique,
        XmNchildType, XmFRAME_TITLE_CHILD,
        XmNfontList, fontlist_diko,
        NULL
    );

    Arg args[10];
    int n = initSetArg(args);

    historique = XmCreateScrolledText(frame_historique, (String) "historique", args, n);

    if (fontlist_diko != NULL) {
        XtVaSetValues(
            historique,
            XmNfontList, fontlist_diko,
            NULL
        );
    }

    XtManageChild(historique);
    XtManageChild(form_principal);

    XtVaSetValues(
        main_window,
        XmNworkWindow, form_principal,
        NULL
    );

    if (fontlist_diko != NULL) {
        XmFontListFree(fontlist_diko);
    }

    return historique;
}

// initialiser le tableau des ressources pour le widget texte
int initSetArg(Arg args[])
{
    int n = 0;

    XtSetArg(args[n], XmNrows, 18);
    n++;

    XtSetArg(args[n], XmNcolumns, 80);
    n++;

    /* ANCIENNE VERSION CONSOLE GRAPHIQUE
       Dans la version console, la zone texte devait rester éditable.

    XtSetArg(args[n], XmNeditable, True);
    n++;

    XtSetArg(args[n], XmNcursorPositionVisible, True);
    n++;
    */

    /* NOUVELLE VERSION HISTORIQUE
       L'historique est une zone de sortie : l'utilisateur ne doit pas modifier
       directement les traductions affichées.
    */
    XtSetArg(args[n], XmNeditable, False);
    n++;

    XtSetArg(args[n], XmNcursorPositionVisible, False);
    n++;

    XtSetArg(args[n], XmNeditMode, XmMULTI_LINE_EDIT);
    n++;

    XtSetArg(args[n], XmNwordWrap, True);
    n++;

    XtSetArg(args[n], XmNscrollHorizontal, False);
    n++;

    return n;
}

// callback de fin ; déclenché par le clic sur le bouton Quit
/*void quit_call(Widget W, XtPointer app_data, XtPointer call_data)
{ printf("Quitting program\n"); exit(0); }*/

// AJOUT 4 - NOUV QUIT CALL CAR DANS MON MAIN INITIAL ON LIBERE LE DICO
void quit_call(Widget W, XtPointer app_data, XtPointer call_data)
{
    (void)W;
    (void)app_data;
    (void)call_data;

    if (dico != NULL) {
        liberer_dictionnaire(dico, taille_dico);
    }

    printf("Au revoir!\n");
    exit(0);
}

// callback empêchant l'insertion et la destruction ailleurs que dans la zone de saisie
// déclenché lors de toute tentative de modifier le texte
void control_insert(Widget W, XtPointer app_data, XtPointer call_data)
{
    (void)W;
    (void)app_data;

    XmTextVerifyCallbackStruct * call = (XmTextVerifyCallbackStruct *) call_data ;

    if ((call->reason == XmCR_MOVING_INSERT_CURSOR and call->newInsert < Start_pos )
         or call->startPos < Start_pos )
    {
        call->doit = False ;
        return ;
    }

    return ;
}

// AJOUT DERNIER +++
void gui_print(Widget text, const char *message)
{
    XmTextPosition pos = XmTextGetLastPosition(text);

    XmTextInsert(text, pos, (char *)message);

    pos = XmTextGetLastPosition(text);
    XmTextShowPosition(text, pos);
    XmTextSetInsertionPosition(text, pos);
}

void rechercher_mot_gui(Widget text,
                        entree *dico,
                        size_t taille_dico,
                        const char *mot,
                        const char *categorie)
{
    int stdout_save = dup(STDOUT_FILENO);

    int tube[2];
    if (pipe(tube) == -1) {
        gui_print(text, "\nErreur : impossible de créer le tube stdout.\n");
        return;
    }

    fflush(stdout);

    dup2(tube[1], STDOUT_FILENO);
    close(tube[1]);

    rechercher_mot(dico, taille_dico, mot, categorie);

    fflush(stdout);

    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);

    char buffer[4096];
    ssize_t nb_lus = read(tube[0], buffer, sizeof(buffer) - 1);
    close(tube[0]);

    if (nb_lus > 0) {
        buffer[nb_lus] = '\0';
        gui_print(text, buffer);
    } else {
        gui_print(text, "Aucune sortie produite.\n");
    }
}

XmFontList creer_fontlist_diko(Widget widget)
{
    XmFontListEntry entree_police = XmFontListEntryLoad(
        XtDisplay(widget),
        (char *)DIKO_FONT,
        XmFONT_IS_FONTSET,
        XmFONTLIST_DEFAULT_TAG
    );

    if (entree_police == NULL) {
        fprintf(stderr, "Erreur : impossible de charger la police DIKO_FONT.\n");
        return NULL;
    }

    XmFontList liste_polices = XmFontListAppendEntry(NULL, entree_police);

    XmFontListEntryFree(&entree_police);

    return liste_polices;
}

void afficher_categories_gui(Widget text)
{
    if (text == NULL) {
        return;
    }

    int stdout_save = dup(STDOUT_FILENO);
    int tube[2];

    if (stdout_save == -1) {
        gui_print(text, "Erreur : impossible de sauvegarder stdout.\n");
        return;
    }

    if (pipe(tube) == -1) {
        gui_print(text, "Erreur : impossible de créer le tube.\n");
        close(stdout_save);
        return;
    }

    fflush(stdout);
    dup2(tube[1], STDOUT_FILENO);
    close(tube[1]);

    
    afficher_categories();

    fflush(stdout);
    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);

    char buffer[8192];
    ssize_t nb_lus = read(tube[0], buffer, sizeof(buffer) - 1);
    close(tube[0]);

    if (nb_lus > 0) {
        buffer[nb_lus] = '\0';
        gui_print(text, buffer);
    }
}

/* NOUVELLE FONCTION
   Callback appelé par le bouton Traduire et par la touche Entrée dans le champ
   de recherche. Elle remplace l'ancien rôle de Done dans l'interface de type
   console graphique.
*/
void traduire_call(Widget W, XtPointer app_data, XtPointer call_data)
{
    (void)W;
    (void)app_data;
    (void)call_data;

    if (champ_recherche == NULL || historique == NULL) {
        return;
    }

    char *commande = XmTextFieldGetString(champ_recherche);

    if (commande == NULL || *commande == '\0') {
        gui_print(historique, "Erreur : aucune saisie. Format attendu : <mot> <categorie>\n\n");
        if (commande != NULL) {
            XtFree(commande);
        }
        return;
    }

    char mot[MAX_LEN];
    char categorie[MAX_LEN];

    
    if (sscanf(commande, "%s", mot) == 1 && (chaines_egales(mot, "H") || chaines_egales(mot, "h"))) {
        gui_print(historique, "> ");
        gui_print(historique, commande);
        gui_print(historique, "\n");

        afficher_categories_gui(historique);

        XmTextFieldSetString(champ_recherche, (char *)"");
        XtFree(commande);
        return;
    }

    
    /*
     * Reprise de la logique de la version non graphique :
     * Aide proposée au moment de la saisie de la catégorie grammaticale.
     */

    if (sscanf(commande, "%s %s", mot, categorie) != 2) {
        gui_print(historique, "> ");
        gui_print(historique, commande);
        gui_print(historique, "\nErreur de saisie. Format attendu : <mot> <categorie>\n");
        gui_print(historique, "Pour afficher l'aide, saisir par exemple : <mot> H ou H\n\n");

        XtFree(commande);
        return;
    }

    if (chaines_egales(categorie, "H") || chaines_egales(categorie, "h")) {
        gui_print(historique, "> ");
        gui_print(historique, commande);
        gui_print(historique, "\n");

        afficher_categories_gui(historique);

        XmTextFieldSetString(champ_recherche, (char *)"");
        XtFree(commande);
        return;
    }

    mettre_en_minuscules(categorie);

    gui_print(historique, "> ");
    gui_print(historique, commande);
    gui_print(historique, "\n");

    rechercher_mot_gui(historique, dico, taille_dico, mot, categorie);

    gui_print(historique, "Recherche effectuée.\n\n");

    XmTextFieldSetString(champ_recherche, (char *)"");
    XtFree(commande);
}   