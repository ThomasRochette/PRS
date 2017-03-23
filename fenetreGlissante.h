#ifndef FENETRE_H

#define FENETRE_H

typedef struct modelFenetre{
	int taille;
	int segmentDebut;
	int dernier;
} FENETRE;


void receptionACK(FENETRE *fen, int numACK, int cont);
FENETRE *initFenetre();

#endif
