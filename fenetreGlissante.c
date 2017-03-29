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

void receptionACK(FENETRE *fen, int numACK, int cont, LIST liste) {
	printf("@FenetreGlissante : ----- RECEPTION ACK ----- \n");
	printf("@FenetreGlissante : On a recu l'ACK %d ! On attendait l'ACK %d ! \n", numACK, liste.premier->num_ACK);
	if(numACK == liste.premier->num_ACK) {
		retirerPremierElem(&liste);
		fen->segmentDebut++;
		if(cont==1){
			fen->taille++;
			fen->dernier = fen->segmentDebut+ fen->taille;
		}
	} else {
		printf("@FenetreGlissante : Erreur ?\n");
		fen->taille = fen->taille / 2;
	}
	printf("@FenetreGlissante : Taille de la fenêtre : %d\n", fen->taille);

}
