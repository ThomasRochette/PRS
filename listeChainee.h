#ifndef __LISTECHAINEE
#define __LISTECHAINEE

typedef struct modelelem{
	int num_ACK;
	char *message;
	double heure_envoie;
	struct modelelem *suivant;
} ELEMLIST;

typedef struct listmod{
	int nbElem;
	ELEMLIST *premier;
} LIST;

LIST initList();
int ajouterElemFin(LIST *liste, double heure, char *message, int numACK);
void retirerPremierElem(LIST *liste);
void afficherListe(LIST *liste);

#endif
