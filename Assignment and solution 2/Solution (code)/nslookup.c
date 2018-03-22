
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  
  if (argc != 2) { //argument count
    perror("Usage: hostnamelookup <hostname>\n"); //to see what the actual error was
    exit(1); //unsucessful termination
  }

  hostname = argv[1]; //contains the name of this computer
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { //get info about the address into res struct
    fprintf(stderr, "Error in resolving hostname %s\n", hostname); //to see what the actual error was
    exit(1); //unsucessful termination
  }
  
  saddr = (struct sockaddr_in*)res->ai_addr; //get struct sockaddr_in with the ip information
  hostaddr = inet_ntoa(saddr->sin_addr); //converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation
  printf("Address for %s is %s\n", hostname, hostaddr);
  exit(0); //sucessful termination
}
