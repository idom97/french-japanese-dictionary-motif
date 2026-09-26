/*=============================================================================
	Nom         : widget.h
	Auteur      : Dominique ERIN
	Rôle        : Déclaration des fonctions et objets liés aux widgets graphiques
	Version     : V01
	Licence     : Réalisé dans le cadre du Cours d'ALGO (2025/2026)
	Compilation : inclus lors de la compilation du programme
	Usage       : #include "widget.h"
=============================================================================*/
#ifndef WIDGET_H
#define WIDGET_H

// pour openmotif
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/MainW.h>
#include <Xm/CascadeB.h>
#include <Xm/RowColumn.h>

// widgets nécessaires à l'organisation graphique
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/LabelG.h>
#include <Xm/TextF.h>
#include <Xm/PushB.h>

#include "commode.h"

// pour l'écriture dans un widget
typedef Widget * output_widget ;

/* =============================================== PROTOTYPES ==============================================*/

Widget initMotifWidgets(Widget, XtAppContext) ;
int initSetArg(Arg []) ;

void quit_call(Widget, XtPointer, XtPointer) ;
void control_insert(Widget, XtPointer, XtPointer) ;

void Done(Widget, XEvent *, String *, Cardinal *) ;

#endif /*WIDGET_H_*/
