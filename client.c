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
  }
  struct sockaddr_in adresse;
  int port= atoi(argv[2]);
  int valid= 1;
  char msg[RCVSIZE];
  char blanmsg[RCVSIZE];


  //create socket
  int desc= socket(AF_INET, SOCK_STREAM, 0);

  // handle error
  if (desc < 0) {
    perror("cannot create socket\n");
    return -1;
  }

  setsockopt(desc, SOL_SOCKET, SO_REUSEADDR, &valid, sizeof(int));

  adresse.sin_family= AF_INET;
  adresse.sin_port= htons(port);
  adresse.sin_addr.s_addr= inet_addr(argv[1]);

  // connect
  int rc= connect(desc, (struct sockaddr*)&adresse, sizeof(adresse));

  // handle error
  if (rc < 0) {
    perror("connect failed\n");
    return -1;
  }


  int cont= 1;
  while (cont) {
    fgets(msg, RCVSIZE, stdin);
    send(desc,msg,strlen(msg),0);
    recv(desc, blanmsg, sizeof(blanmsg),0);
    printf("%s",blanmsg);
    memset(blanmsg,0,RCVSIZE);
    if (strcmp(msg,"stop\n") == 0) {
      cont= 0; 
    }
  }

close(desc);
return 0;
}
