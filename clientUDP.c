#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RCVSIZE 1024

int main (int argc, char *argv[]) {

  if(argc!=3){
    printf("Argument invalid\n");
    exit(1);
  }

  struct sockaddr_in adresse;
  int port= atoi(argv[2]);
  int valid= 1,i;
  socklen_t alen= sizeof(adresse);
  char blanmsg[RCVSIZE];
  char sport[4];// String du port


  //create socket
  int desc= socket(AF_INET, SOCK_DGRAM, 0);

  // handle error
  if (desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port);
  adresse.sin_addr.s_addr= inet_addr(argv[1]);

  int cont= 0;// On ne rentre pas dans la boucle sans la connexion
  printf("Tentative de connexion:\n\tEnvoi du SYN\n");

  sendto(desc,"SYN",strlen("SYN"),0,(struct sockaddr*)&adresse, sizeof(adresse));
  //Attente du SYN-ACK
  recvfrom(desc, blanmsg, sizeof(blanmsg),0,(struct sockaddr*)&adresse, &alen);

  if(strcmp(blanmsg,"SYN-ACK")==0){
    printf("\tReception du SYN-ACK\n\tEnvoi de l'ACK\n");
    sendto(desc,"ACK",strlen("ACK"),0,(struct sockaddr*)&adresse, sizeof(adresse));

    cont=1;
  }else{
    printf("La connexion n'est pas établie\n");
  }

  memset(blanmsg,0,RCVSIZE);
  recvfrom(desc, blanmsg, sizeof(blanmsg),0,(struct sockaddr*)&adresse, &alen);
  //Réception du port pour communiquer
  strncpy(sport,blanmsg,4);
  port=atoi(sport);
  adresse.sin_port= htons(port);//Changement du port du socket
  //1 seul socket sur le client

  printf("\n\t---CONNEXION ETABLIE---\n\n");
  memset(blanmsg,0,RCVSIZE);
  int ack=0;//Compteur d'ACK
  //int ack_ok=1;
  char ack_char[6];// sting de l'ACK envoyer
  char ack_send[4]="ACK_";// string de l'ACK qui doit êtred reçus

  while (cont==1) {
    memset(ack_char,0,RCVSIZE);
    recvfrom(desc, blanmsg, sizeof(blanmsg),0,(struct sockaddr*)&adresse, &alen);
    printf("%s\n",blanmsg);

    i=0;
    while (blanmsg[i]!='_') {
      ack_char[i]=blanmsg[i];
      i++;
    }
    printf("%s\n", ack_char);

    ack = atoi(ack_char);
    printf("ACK : %d\n", ack);

    sprintf(ack_send,"%s","ACK_");
    strcat(ack_send,ack_char);
    strcat(ack_send,"_");
    printf("%s\n",ack_send);

    sendto(desc,ack_send,strlen(ack_send),0,(struct sockaddr*)&adresse, sizeof(adresse));
    sleep(1);
  }

close(desc);
return 0;
}
