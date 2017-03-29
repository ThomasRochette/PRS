#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listeChainee.h"


LIST initList(){
	LIST *liste;
	liste=(LIST*)malloc(sizeof(LIST));
	liste->nbElem=0;
	liste->premier=NULL;
	return *liste;
}


int ajouterElemFin(LIST *liste, double heure, char * chaine, int numACK){
		ELEMLIST *newelem, *visitor;
		newelem=(ELEMLIST*)malloc(sizeof(ELEMLIST));
		newelem->message = malloc(sizeof(chaine));
		visitor=(ELEMLIST*)malloc(sizeof(ELEMLIST));
		visitor=liste->premier;
		newelem->num_ACK = numACK;
		strcpy(newelem->message, chaine);
		newelem->heure_envoie = heure;
		newelem->suivant=NULL;


		if(liste->premier==NULL){
			liste->premier=newelem;
			printf("GOD:%d\n", newelem->num_ACK);
			liste->nbElem++;
		}else{
			printf("Test : %d et la : %d\n", visitor->num_ACK, liste->premier->num_ACK);
			printf("On va entrer dans le for ! \n");
			for(int i=0; i<liste->nbElem-1; i++) {
				printf("On est dans le for ! \n");
				printf("Test : %d\n", visitor->num_ACK);
				visitor=visitor->suivant;
				//printf("%d\n", visitor->suivant->num_ACK);
			}
			printf("On sort du for ! \n");
			visitor->suivant=newelem;
			liste->nbElem++;
		}
		afficherListe(liste);
		printf("@ListeChainee : Sortie d'ajouter ! \n");
		free(visitor);
		return 0;
}

void retirerPremierElem(LIST *liste){
	if(liste->premier == NULL) {
		printf("@ListeChainee : Erreur : liste vide !\n");
		exit(-1);
	}
	ELEMLIST *premier;
	//ELEMLIST *suivant;
	premier=(ELEMLIST*)malloc(sizeof(ELEMLIST));
	//suivant=(ELEMLIST*)malloc(sizeof(ELEMLIST));
	premier=liste->premier;
	liste->premier=premier->suivant;
	//liste->premier->suivant = NULL;
	liste->nbElem--;
	free(premier);
}

void afficherListe(LIST *liste){
	ELEMLIST *element;
	element=(ELEMLIST*)malloc(sizeof(ELEMLIST));
	element=liste->premier;
	int i;
	printf("Taille liste : %d\n", liste->nbElem);
	for(i=0; i<liste->nbElem; i++){
		if(element->suivant!=NULL){
			printf("NUMERO_SEQ : %d, HEURE_ENVOIE : %f, MESSAGE : %s\n", element->num_ACK, element->heure_envoie, element->message);
			element=element->suivant;
		}else{
			printf("NUMERO_SEQ : %d, HEURE_ENVOIE : %f, MESSAGE : %s\n", element->num_ACK, element->heure_envoie, element->message);
		}
	}
}


/*int getTaille(LIST *liste) {
	if(liste->premier==NULL) {
		return 0;
	} else {
		ELEMLIST *element;
		element=(ELEMLIST*)malloc(sizeof(ELEMLIST));
		element = liste->premier;
		int taille = 1;
		while(element->suivant != NULL) {
			taille ++;
			element = element->suivant;
		}
		return taille;
	}
}*/
