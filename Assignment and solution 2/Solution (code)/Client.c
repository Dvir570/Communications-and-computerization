#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
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
  int socket_fd, cc, ccc, dsize;
  struct sockaddr_in  s_router, s_in; //struct with information about destination IP
  struct hostent *hostptr;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //socket decriptor for UDP protocol using IPv4
  
  memset((char *) &s_router,0, sizeof(s_router)); /* They say you must do this */ //make all the dest information equal to zero
  hostptr = gethostbyname(argv[1]); //return IP address of this computer by giving name of the computer
  s_router.sin_family = (short) AF_INET; //for IPv4 family
  memcpy(hostptr->h_addr, (char *)&s_router.sin_addr,hostptr->h_length);
  s_router.sin_port = 2222; //convert from host byte order to network byte order with short size
  
while(1){
  char message[500];
  printf("enter message: \n");
  scanf ("%[^\n]%*c", message);
  if(strcmp(message, "exit") == 0){
    printf("GOODBYE!\n");
    exit(0);
  }

  s_in.sin_family = (short)AF_INET; //for IPv4 family
  s_in.sin_addr.s_addr = htonl(INADDR_ANY); //convert address from host byte order to network byte order with long size
  s_in.sin_port = 1111; //convert port from host byte order to network byte order with short size
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));  //conect address and port with the socket
  

  sendto(socket_fd,message,500,0,(struct sockaddr *)&s_router,sizeof(s_router)); //send infornation to dest address and port
  printf("MESSAGE SENT TO ROUTER\n");
  printsin( &s_in, "CLIENT ON LISTENING", "Local socket is:");
  char messagein[500];
  
  dsize = sizeof(s_router);
  cc = recvfrom(socket_fd,messagein,500,0,(struct sockaddr *)&s_router,&dsize);  //function that may recive information
  printsin( &s_router, "MESSAGE RECIVED FROM:", "Local socket is:");
  printf("MESSAGE: %s\n",messagein);
  fflush(stdout);
}
  return 0;
}
