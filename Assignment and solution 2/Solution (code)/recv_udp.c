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
  struct sockaddr_in  s_in, from; //struct to specify a address to which to connect a socket
  //struct { char head; char*  body; char tail;} msg;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //socket decriptor for UDP protocol using IPv4

  memset((char *) &s_in,0, sizeof(s_in));  /* They say you must do this    */ //make all the s_in information equal to zero

  s_in.sin_family = (short)AF_INET; //for IPv4 family
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */ //convert address from host byte order to network byte order with long size
  s_in.sin_port = htons((u_short)0x3333); //convert port from host byte order to network byte order with short size

  printsin( &s_in, "RECV_UDP", "Local socket is:"); 
  //fflush(stdout);

  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in));  //conect address and port with the socket
  char name[100];
  ccc=cc;
  while(ccc==cc) {  // keep in listen till recive information
    fsize = sizeof(from);
    cc = recvfrom(socket_fd,name,100/*&msg,sizeof(msg)*/,0,(struct sockaddr *)&from,&fsize);  //function that may recive information
    printsin( &from, "recv_udp: ", "Packet from:");
    //printf("Got data ::%c%s%c\n",msg.head,/*(long) ntohl(*/msg.body/*)*/,msg.tail); 
    printf("%s\n",name);
    fflush(stdout);
  }

  sendto(socket_fd,name,100,0,(struct sockaddr *)&from,
                  sizeof(from));
  
  
  return 0;
}

