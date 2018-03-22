#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int socket_fd, cc, ccc, dsize;
  struct sockaddr_in  dest, from; //struct with information about destination IP
  struct hostent *hostptr;
 // struct { char head; char* body; char tail; } msgbuf;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //socket decriptor for UDP protocol using IPv4
  
  memset((char *) &dest,0, sizeof(dest)); /* They say you must do this */ //make all the dest information equal to zero
  hostptr = gethostbyname(argv[1]); //return IP address of this computer by giving name of the computer
  dest.sin_family = (short) AF_INET; //for IPv4 family
  memcpy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  dest.sin_port = htons((u_short)0x3333); //convert from host byte order to network byte order with short size
  
  //msgbuf.head = '<';
  //msgbuf.body = "dvir";/*htonl(*///); /* IMPORTANT! */  //convert from host byte order to network byte order with long size
  //msgbuf.tail = '>';
  
  char name[100] = "dvir";

  //printf("%c",s);
  sendto(socket_fd,name,100,0,(struct sockaddr *)&dest,
                  sizeof(dest)); //send infornation to dest address and port
                  
  
  from.sin_family = (short)AF_INET; //for IPv4 family
  from.sin_addr.s_addr = htonl(INADDR_ANY); //convert address from host byte order to network byte order with long size
  from.sin_port = htons((u_short)0x3333); //convert port from host byte order to network byte order with short size
  bind(socket_fd, (struct sockaddr *)&from, sizeof(from));  //conect address and port with the socket
  char namein[100];
  ccc=cc;
  while(ccc==cc) {  // keep in listen till recive information
  dsize = sizeof(dest);
  cc = recvfrom(socket_fd,namein,100,0,(struct sockaddr *)&dest,&dsize);  //function that may recive information
  printf("%s\n",namein);
  fflush(stdout);
}

  return 0;
}
