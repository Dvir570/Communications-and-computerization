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
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

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
  int socket_fd, cc, fsize;
  double rnd;
  double x;
  struct sockaddr_in  s_in, from, s_client, s_server; //struct to specify a address to which to connect a socket
  struct hostent *hostptr;

  /* Intializes random number generator */
  srand(time(NULL));
  
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //socket decriptor for UDP protocol using IPv4

  memset((char *) &s_in,0, sizeof(s_in));  /* They say you must do this    */ //make all the s_in information equal to zero

  s_in.sin_family = (short)AF_INET; //for IPv4 family
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */ //convert address from host byte order to network byte order with long size
  s_in.sin_port = 2222; //convert port from host byte order to network byte order with short size

  fflush(stdout);
  
  memset((char *) &s_client,0, sizeof(s_client));  /* They say you must do this    */ //make all the s_in information equal to zero

  s_client.sin_family = (short)AF_INET; //for IPv4 family
  hostptr = gethostbyname(argv[1]); //return IP address of this computer by giving name of the computer
  s_client.sin_family = (short) AF_INET; //for IPv4 family
  memcpy(hostptr->h_addr, (char *)&s_client.sin_addr,hostptr->h_length);
  s_client.sin_port = 1111; //convert port from host byte order to network byte order with short size

  memset((char *) &s_server,0, sizeof(s_server));  /* They say you must do this    */ //make all the s_in information equal to zero

  s_server.sin_family = (short)AF_INET; //for IPv4 family
  hostptr = gethostbyname(argv[1]); //return IP address of this computer by giving name of the computer
  memcpy(hostptr->h_addr, (char *)&s_server.sin_addr,hostptr->h_length);
  s_server.sin_port = 3333; //convert port from host byte order to network byte order with short size

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));  //conect address and port with the socket
  char messagein[500];
  printf("choose a number X from range [0,1]: ");
  scanf("%lf",&x);
  printsin( &s_in, "ROUTER ON LISTENING", "Local socket is:"); 
  while(1) {  // keep in listen till recive information
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,messagein,500/*&msg,sizeof(msg)*/,0,(struct sockaddr *)&from,&fsize);  //function that may recive information
    printsin( &from, "MESSAGE RECIVED FROM:", "Local socket is:");
    printf("MESSAGE: %s\n",messagein);
    //fflush(stdout);
    if(from.sin_port == s_client.sin_port){
        
        /* make random number */
        rnd = (double)rand()/RAND_MAX;
        printf("random: %f\n", rnd);
        if(rnd>x){
          sendto(socket_fd,messagein,500,0,(struct sockaddr *)&s_server,sizeof(s_server));
          printf("MESSAGE SENT TO SERVER\n");
        } 
        else{
          printf("THE MESSAGE DIDN'T FORWARD TO SERVER\n");
        }
    }
    else if(from.sin_port == s_server.sin_port){
        sendto(socket_fd,messagein,500,0,(struct sockaddr *)&s_client,sizeof(s_client));
                  printf("MESSAGE SENT TO CLIENT\n");
    }
  }
  return 0;
}

