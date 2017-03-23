#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "fenetreGlissante.h"

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

	if(argc!=2){
		printf("Argument invalid\n");
	}

	FENETRE *fen = initFenetre();
	fd_set Ldesc;
	struct sockaddr_in adresseTCP, adresseUDP, adresseDATA, client;
	int port= atoi(argv[1]);
	int valid= 1;
	int tailleEnvoie=0;
  int i,j;
	socklen_t alen= sizeof(client);
	char buffer[RCVSIZE];
	char ACK[RCVSIZE];//string de l'ACK
	char sport[4];//strinf du port

	//create socket
	int descTCP= socket(AF_INET, SOCK_STREAM, 0);//Socket TCP
	int descUDP= socket(AF_INET, SOCK_DGRAM, 0);//Socket UDP principal



	// handle error
	if (descTCP < 0) {
		perror("cannot create socket\n");
		return -1;
	}

	if (descUDP < 0) {
		perror("cannot create socket\n");
		return -1;
	}

	//Socket TCP
	setsockopt(descTCP, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

	adresseTCP.sin_family= AF_INET;
	adresseTCP.sin_port= htons(port);
	adresseTCP.sin_addr.s_addr= htonl(INADDR_ANY);

	//Socket UDP principal
	setsockopt(descUDP, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

	adresseUDP.sin_family= AF_INET;
	adresseUDP.sin_port= htons(port+1);
	adresseUDP.sin_addr.s_addr= htonl(INADDR_ANY);


	if (bind(descTCP, (struct sockaddr*) &adresseTCP, sizeof(adresseTCP)) == -1) {
		perror("Bind fail\n");
		close(descTCP);
		return -1;
	}

	if (bind(descUDP, (struct sockaddr*) &adresseUDP, sizeof(adresseUDP)) == -1) {
		perror("Bind fail\n");
		close(descUDP);
		return -1;
	}

	if (listen(descTCP, 5) < 0) {
		printf("Listen failed\n");
		return -1;
	}

	while (1) {
		FD_ZERO(&Ldesc);
		FD_SET(descTCP,&Ldesc);
		FD_SET(descUDP,&Ldesc);
		select(descUDP+1,&Ldesc,NULL,NULL,NULL);

		if(FD_ISSET(descTCP,&Ldesc)==1){
			printf("activité TCP\n");
			int rqst= accept(descTCP, (struct sockaddr*)&client, &alen);
			int pid=fork();
			if(pid==0){

				close(descTCP);

				int msgSize= recv(rqst,buffer,RCVSIZE,0);

				while (msgSize > 0) {
					send(rqst,buffer,msgSize,0);
					printf("%s\n",buffer);
					memset(buffer,0,RCVSIZE);
					msgSize= recv(rqst,buffer,RCVSIZE,0);
				}
				close(rqst);
				exit(0);
			}

			close(rqst);
		}
		if(FD_ISSET(descUDP,&Ldesc)==1){
			printf("Activité UDP détectée\n");

			int cont=0;//Pas de connection =  pas de boucle infini
			memset(buffer,0,RCVSIZE);//on vide le buffer

			//On attend un message
			recvfrom(descUDP,buffer,RCVSIZE,0,(struct sockaddr*)&client, &alen);
			printf("%s\n",buffer);

			//SI c'est un message de synchronisation
			if(strncmp(buffer,"SYN",3)==0){
				printf("Etablissement d'une donnexion\n\tEnvoi du SYN-ACK\n");
				sendto(descUDP,"SYN-ACK",sizeof("SYN-ACK"),0,(struct sockaddr*)&client, alen);
				//Attente de l'ACK
				recvfrom(descUDP,buffer,RCVSIZE,0,(struct sockaddr*)&client, &alen);

				if(strcmp(buffer,"ACK")==0){
					printf("\tReception de l'ACK\n");
					cont=1;//On peut rentrer dans la boucle
				}
			}else{
				printf("Connexion non établie\n");
				exit(0);
			}

			//Création d'un socket pour gérer le multi-client
			int portClient = port+2;//Port du socket à incémenter
			int descClient= socket(AF_INET, SOCK_DGRAM, 0);//Socket UDP du client
			if (descClient < 0) {
				perror("cannot create socket\n");
				return -1;
			}
			setsockopt(descClient, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

			adresseDATA.sin_family= AF_INET;
			adresseDATA.sin_port= htons(portClient);
			adresseDATA.sin_addr.s_addr= htonl(INADDR_ANY);

			if (bind(descClient, (struct sockaddr*) &adresseDATA, sizeof(adresseDATA)) == -1) {
				perror("Bind fail\n");
				close(descClient);
				return -1;
			}

			//Envoi de son port au client pour terminer la connexion
			sprintf(sport,"%d",portClient);
      printf("%s\n",sport);
			sendto(descUDP,sport,sizeof(sport),0,(struct sockaddr*)&client, alen);

			//Gestion du nouveau client dans un processus fils
			int pid = fork();

			if(pid==0){
				close(descUDP);//On utilise seulement le nouveau socket
				printf("\n\t---CONNEXION ETABLIE---\n\n");
				memset(buffer,0,RCVSIZE);

				FILE *file;
				fd_set Ludp;
				char segment_data[122];
				char segment[6];
				int num_seg=0;
				char tmp;

				struct timeval tv;
				tv.tv_sec = 0;
				tv.tv_usec = 0;

				file = fopen("test.txt","r");
				if(file==NULL){
					printf("Erreur d'ouverture du fichier\n");
					exit(1);
				}

				while (1) {
					memset(buffer,0,RCVSIZE);
					memset(ACK,0,RCVSIZE);
          memset(segment,0,RCVSIZE);
          memset(segment_data,0,RCVSIZE);

					FD_SET(descClient, &Ludp);
					select(descClient+1, &Ludp, NULL, NULL, &tv);

					if(FD_ISSET(descClient, &Ludp)){
						recvfrom(descClient,buffer,RCVSIZE,0,(struct sockaddr*)&client, &alen);
						tailleEnvoie++;
						i=4;
            j=0;
						while(buffer[i]!='_'){
							ACK[j]=buffer[i];
							i++;
              j++;
						}

						receptionACK(fen, atoi(ACK), cont);
						printf("ACK_%s\n", ACK);

            if(cont==0 && atoi(ACK)==fen->dernier){
              printf("Fermeture de la connexion\n" );
              close(descClient);
              exit(0);
            }
					}else{
						if(fen->dernier > num_seg) {

							printf("On a pas d'ACK, on va envoyer ! dernier : %d, num_Seq : %d \n", fen->dernier, num_seg);
							sprintf(segment,"%d_",num_seg);

							for(i=0; i<122; i++){
								tmp = fgetc(file);
								if(tmp != EOF) {
									segment_data[i] = tmp;
								} else {
                  fen->dernier=num_seg;
									cont = 0;
								}
							}

							strcat(segment, segment_data);
              printf("%s\n",segment);
              printf("\n");

							sendto(descClient, segment, strlen(segment), 0, (struct sockaddr*)&client, sizeof(client));
              num_seg++;
						}else{
							printf("La fenetre nous bloque, on doit attendre ! \n");
              sleep(1);
						}
					}
				}
				close(descClient);// Fermeture dans le père du socket de communication avec le client
				descClient++;// Incrémentation du port pour le client suivant
			}
		}
	}
  close(descTCP);
  close(descUDP);
  return 0;
}
