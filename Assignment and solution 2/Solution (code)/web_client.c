
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
#define PORT 1337 

int main(int argc, char* argv[])
{ 
  int sock; 
  struct sockaddr_in cli_name; 
  int count;
  int value;
  char* port = "80";
  struct addrinfo* res;
  char* url;
  char* path;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;
  
  if (argc != 2) { //argument count
    perror("Usage: hostnamelookup <hostname>\n"); //to see what the actual error was
    exit(1); //unsucessful termination
  }

  url = argv[1]; //contains the URL of web site
  //create a http request message from the url
  char request[500] = "GET ";
  strcat(request,url);
  strcat(request," HTTP/1.0\nHOST: ");
  hostname = strtok (url,"/");
  hostname = strtok (NULL, "/");
  path = strtok (NULL, "");
  if(path == NULL)
    path = "";
  if (strcspn (hostname,":")!=strlen(hostname))
       {
          port = strtok (hostname, ":");
          port = strtok (NULL, ":");
       }
  printf("path = %s\n",path);
  printf("port = %s\n",port);
  printf("hostname = %s\n",hostname);
  strcat(request,hostname);
  strcat(request,"\n\n");
  printf("%s",request);
  
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
  cli_name.sin_port = htons(atoi(port));//convert port from host byte order to network byte order with short size
  
 
  
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0){ //check if create connection with server was seccess
      perror ("Error establishing communications"); //to see what the actual error was
      close(sock); //close the socket
      exit(1); //unsucessful termination
    }
    
    //write request into buffer
    int total = strlen(request);
    int sent = 0, bytes;
    do {
        bytes = write(sock,request+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0) //end writing
            break;
        sent+=bytes;
    } while (sent < total);
    
    //reed response from buffer
    char response[5000];
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    int received = 0;
    do {
        bytes = read(sock,response+received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0) //end reading
            break;
        received+=bytes;
    } while (received < total);
    
    close(sock); // close the socket
  
  printf("\nResponse:\n%s\n",response);
  printf("\nExiting now.\n");
  exit(0);  //sucessful termination

} 
