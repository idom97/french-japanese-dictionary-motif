#ifndef DICO_C_HEADERS_H
#define DICO_C_HEADERS_H

// l'ordre compte
#include "widget.h"
#include "commode.h"

// Headers système utilisés par dico_gui.cpp
#include <unistd.h>
#include <fcntl.h>
#include <locale.h>

#ifdef __cplusplus
// DERNIER AJOUT SINON ERREUR MAKEFILE
extern "C" {
#endif

#include "structures.h"
#include "sys.h"
#include "parse.h"
#include "recherche.h"
#include "init.h"
#include "free.h"
#include "cons.h"
#include "main_initial.h"


#ifdef __cplusplus
}
#endif

#endif
