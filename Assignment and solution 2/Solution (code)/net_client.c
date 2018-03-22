
#include <sys/types.h>
#include <unistd.h>    
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define SIM_LENGTH 10 
//#define IP_ADDRESS "172.17.0.95" 
#define PORT 1337 

int main(int argc, char* argv[])
{ 
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value; 
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
  printf("Client is alive and establishing socket connection.\n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0); //create socket decriptor for TCP protocol using IPv4
  if (sock < 0){ //check if didnt create the socket
      perror ("Error opening channel"); //to see what the actual error was
      close(sock);//close the socket
      exit(1); //unsucessful termination
    }
      

  memset((char *)&cli_name,0, sizeof(cli_name));  //make all sockaddr_in properties equal to zero
  cli_name.sin_family = AF_INET;  //for IPv4 family
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); //converts the Internet host address from IPv4 numbers-and-dots notation into binary data in network byte order. 
  cli_name.sin_port = htons(PORT);//convert port from host byte order to network byte order with short size


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0){ //check if create connection with server was seccess
      perror ("Error establishing communications"); //to see what the actual error was
      close(sock); //close the socket
      exit(1); //unsucessful termination
    }

  
  for (count = 1; count <= SIM_LENGTH; count++){
      read(sock, &value, 4); //read data into a buffer
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  close(sock); //close the sock
  exit(0);  //sucessful termination

} 
