#include <stdio.h>
#include <stdlib.h>
#include "fenetreGlissante.h"

FENETRE *initFenetre() {
	FENETRE *fen = malloc(sizeof(FENETRE));
	fen->segmentDebut = 1;
	fen->taille = 10;
	fen->dernier = fen->segmentDebut + fen->taille;
	return fen;
}

void receptionACK(FENETRE *fen, int numACK, int cont) {
	if(numACK == fen->segmentDebut) {
		printf("@FenetreGlissante : On a recu l'ACK qu'on attendait ! \n");
		fen->segmentDebut++;
		if(cont==1){
			fen->dernier = fen->segmentDebut+ fen->taille;
		}
	}
}
