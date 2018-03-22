#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printsin(struct sockaddr_in *sin, char *pname, char* msg){
  char *ip= inet_ntoa(sin->sin_addr);
  printf("%s\n", pname);
  printf("%s", msg);
  
  printf(" ip= %s, port= %d", ip, sin->sin_port);
  //-- port: sin->sin_port (host integer type) 
  //-- IP: sin->sin_addr (IP in dotted-decimal notation) 
  printf("\n");
}

int main(int argc, char *argv[])
{
  int socket_fd, cc, ccc, fsize;
  struct sockaddr_in  s_router, s_in; //struct to specify a address to which to connect a socket


  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //socket decriptor for UDP protocol using IPv4

  memset((char *) &s_in,0, sizeof(s_in));  /* They say you must do this    */ //make all the s_in information equal to zero

  s_in.sin_family = (short)AF_INET; //for IPv4 family
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */ //convert address from host byte order to network byte order with long size
  s_in.sin_port = 3333; //convert port from host byte order to network byte order with short size

  printsin( &s_in, "SERVER ON LISTENING", "Local socket is:"); 
  //fflush(stdout);

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));  //conect address and port with the socket
  char messagein[500];
  while(1){

    fsize = sizeof(s_router);
    cc = recvfrom(socket_fd,messagein,500,0,(struct sockaddr *)&s_router,&fsize);  //function that may recive information
    printsin( &s_router, "MESSAGE RECIVED FROM:", "Local socket is:");

    printf("MESSAGE: %s\n",messagein);
    fflush(stdout);

  
  char message[500];
  printf("enter message: \n");
  scanf ("%[^\n]%*c", message);
  if(strcmp(message, "exit") == 0){
    printf("GOODBYE!\n");
    exit(0);
  }
  sendto(socket_fd,message,500,0,(struct sockaddr *)&s_router,sizeof(s_router));
  printf("MESSAGE SENT TO ROUTER\n");
  
}
  return 0;
}

