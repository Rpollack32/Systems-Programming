#include <netdb.h> 
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
#include <arpa/inet.h> 
#include <sys/socket.h> 
#define MAX 80 
//#define PORT 8080 
#define SA struct sockaddr 
void func(int sockfd) 
{ 
    char buff[MAX]; 
    int n; 
    while(1) { 
        bzero(buff, sizeof(buff)); 
        printf("Response: "); 
        n = 0; 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
        write(sockfd, buff, sizeof(buff)); 
        bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("Server : %s\n", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
} 
  
//int main() 
void main(int argc,void *argv[]){

   int portNumber = atoi(argv[1]);
    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
  
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(portNumber); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
        char buff[MAX]; 
        read(sockfd, buff, sizeof(buff)); 
        printf("Server: %s\n", buff); 

    // function for communication 
    func(sockfd); 
  
    // close the socket 
    close(sockfd); 
}
