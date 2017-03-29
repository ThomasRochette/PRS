#ifndef FENETRE_H
#define FENETRE_H

#include "listeChainee.h"

typedef struct modelFenetre{
	int taille;
	int segmentDebut;
	int dernier;
} FENETRE;


void receptionACK(FENETRE *fen, int numACK, int cont, LIST liste);
FENETRE *initFenetre();

#endif
