#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 
#define PORT 1337

void clean_up(int cond, int *sock)
{ printf("Exiting now.\n");
  close(*sock); //close the socket
  exit(cond); //termination
} 

int main(void)
{ 
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  socklen_t len; 
  int count;
  
  sock = socket(AF_INET, SOCK_STREAM, 0);   //create socket decriptor for TCP protocol using IPv4
  if (sock < 0){ //check if didnt create the socket
      perror ("Error opening channel"); //to see what the actual error was
      clean_up(1, &sock); //unsucessful termination
    }
  
  memset((char *)&serv_name,0, sizeof(serv_name)); //make all sockaddr_in properties equal to zero
  serv_name.sin_family = AF_INET; //for IPv4 family                                   
  serv_name.sin_port = htons(PORT); //convert port from host byte order to network byte order with short size 
  
  if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0){ //chek if the conect address and port with the socket secceed
      perror ("Error naming channel"); //to see what the actual error was
      clean_up(1, &sock); //unsucessful termination
    }
      
  printf("Server is alive and waiting for socket connection from client.\n");
  
  listen(sock, 1); //wating for 1 connect

  
  len = sizeof(serv_name); //sive of this struct
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len); //accapt the first request for connection

  for (count = 1; count <= SIM_LENGTH; count++)
    { 
	    write(connect_sock, &count, 4); //write data out of a buffer.
      printf("Server has written %d to socket.\n", count);
    }

  close(connect_sock); //close the connect_sock
  clean_up(0, &sock); //sucessful termination

}
