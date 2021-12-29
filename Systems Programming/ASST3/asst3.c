#include <stdio.h> 
#include <unistd.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 

#define MAX 80 
#define SA struct sockaddr 


/*
 * This method checks what is wrong with the client response and prints out an error to standard out
 * 
 * This method prints out the textual description of the error to standard out for the issue associated with the client response
 * @param int msgCounter
 * @param char *msgType
 * @param char *msgSize
 * @param char *msgReponse
 */
void sendClientErrorMessage(int msgCounter, char *msgType, char msgSize, char *msgResponse){

   int length=0;
   length = strlen(msgResponse);

   char *punct="?";

   // Determine that client response is valid for the "Who's there?" response
   if (msgCounter == 3){
     punct=".";
   }

   if (!strstr(msgResponse, punct)){
     fprintf(stdout, "message %d content was not correct (i.e. missing punctuation)\n", msgCounter);
     return;
   }

   if (length != msgSize){
     fprintf(stdout, "message %d length value was incorrect (i.e. should be %d characters long\n", msgCounter, msgSize);
     return;
   }
 
}

/*
 * This method handles the "ERR" response from the client and prints out the textual description of the error to standard out.
 * @param int msgCounter
 * @param char *msgReponse
 */
void handleErrorFromClient(int msgCounter, char *msgResponse){

   // Message 0 errors
   if (strcmp("M0CT", msgResponse) == 0){
     fprintf(stdout, "message 0 content was not correct\n");
   }

   if (strcmp("M0LN", msgResponse) == 0){
     fprintf(stdout, "message 0 length value was incorrect (i.e. should be 13 characters long\n");
   }

   if (strcmp("M0FT", msgResponse) == 0){
     fprintf(stdout, "message 0 format was broken (did not include a message type, missing or too many'|'\n");
   }

   // Message 2 errors
   if (strcmp("M2CT", msgResponse) == 0){
     fprintf(stdout, "message 2 content was not correct (i.e. missing punctuation\n");
   }

   if (strcmp("M2LN", msgResponse) == 0){
     fprintf(stdout, "message 2 length value was incorrect (i.e. should be length of the message)\n");
   }

   if (strcmp("M2FT", msgResponse) == 0){
     fprintf(stdout, "message 2 format was broken (did not include a message type, missing or too many'|'\n");
   }

   // Message 4 errors
   if (strcmp("M4CT", msgResponse) == 0){
     fprintf(stdout, "message 4 content was not correct (i.e. missing punctuation\n");
   }

   if (strcmp("M4LN", msgResponse) == 0){
     fprintf(stdout, "message 4 length value was incorrect (i.e. should be length of the message)\n");
   }

   if (strcmp("M4FT", msgResponse) == 0){
     fprintf(stdout, "message 4 format was broken (did not include a message type, missing or too many'|'\n");
   }

}

/*
 * Process the client message and respond
 * @param: int sockfd
 */
void processResponse(int sockfd){ 

    int msgCounter=-1;
    char buffer[MAX]; 
    char *msgType, msgSize, *msgResponse;
    char response[MAX];
    int n; 
    // infinite loop for listening to client responses
    while(1) { 
        bzero(buffer, MAX); 
        msgCounter=msgCounter+2;
  
        // read the message from client and copy it in buffer 
        read(sockfd, buffer, sizeof(buffer)); 

        // Break apart response
        msgType = strtok(buffer,"|");
        
        // Check if the message being with either REG or ERR, if not print error to stdout
        if ((strcmp("ERR", msgType) != 0) && (strcmp("REG", msgType) != 0)) {
           fprintf(stdout, "Message %d format was broken ( did not include message type, missing or too many'|'\n", msgCounter);
          return;
        }

        // Check if we received an error response
        if (strcmp("ERR", msgType) == 0) {
          msgResponse = strtok(NULL,"|");
          handleErrorFromClient(msgCounter, msgResponse);
          break;
        }

        // Parse the rest of the message from the client
        msgSize = atoi(strtok(NULL,"|"));
        msgResponse = strtok(NULL,"|");

        // If the client message contains "close" or "free" or "retrun then shutdown the server
        if ((strcmp("close", msgResponse) == 0) || (strcmp("free", msgResponse) == 0) || (strcmp("return", msgResponse) == 0)){ 
            break; 
        }

        if (strcmp(msgResponse, "Who's there?") == 0){
          strcpy(response, "REG|4|Who.|");
          write(sockfd, response, sizeof(response));
        }else if (strcmp(msgResponse, "Who, who?") == 0){
          strcpy(response, "REG|29|I didn't know you were an owl|");
          write(sockfd, response, sizeof(response));
        } else if (strcmp(msgResponse, "Ugh.") == 0){
        // Determine if we are at the end of the joke. If so, start the process again
          strcpy(response, "REG|13|Knock, Knock.|");
          write(sockfd, response, sizeof(response));
        } else {
        // Check that the response from the client is correct. If not, output a textual description of the error on standard out.
          sendClientErrorMessage(msgCounter, msgType, msgSize, msgResponse);
          break;
        }
    } 
} 

void main(int argc,void *argv[]){

   int sockfd, connfd, length; 
   struct sockaddr_in serverAddress, client; 

   // Verify that a port number was requested to listen on 
   if (argc != 2) {
     printf("Please request a port to listen on\n");
     exit(0);
   }
 
   int portNumber = atoi(argv[1]);
   // Verify that the request port to listen on is between 5000 and 65536
   if (portNumber < 5000 || portNumber > 65536) {
     printf("Please request a port to listen above 5000 and less than 65536\n"); 
     exit(0);  
   }

   // Create socket 
   sockfd = socket(AF_INET, SOCK_STREAM, 0); 
   if (sockfd == -1) { 
     printf("Socket creation failed...\n"); 
     exit(0); 
   }else{
     printf("Socket successfully created..\n");
   }
 
   bzero(&serverAddress, sizeof(serverAddress)); 
  
   // assign IP, PORT 
   serverAddress.sin_family = AF_INET; 
   serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); 
   serverAddress.sin_port = htons(portNumber); 
  
    // Binding newly created socket to given IP and verification 
   if ((bind(sockfd, (SA*)&serverAddress, sizeof(serverAddress))) != 0){ 
     printf("Socket binding failed...\n"); 
     exit(0); 
   } 

   // Now server is ready to listen and verification 
   if ((listen(sockfd, 5)) != 0){ 
     printf("Lister failed...\n"); 
     exit(0); 
   }else{
     printf("Listening...\n"); 
   }
  
   // Accept the data packet from client 
   length = sizeof(client); 
   connfd = accept(sockfd, (SA*)&client, &length); 
   if (connfd < 0) { 
     printf("Failed to accept the client connection...\n"); 
     exit(0); 
   }else{
     printf("Server has acccepted the client connection...\n"); 
   }

   // Send the initial "Knock, Knock" to the client
   char sendBuffer[25];
   strcpy(sendBuffer, "REG|12|Knock, Knock|\n");
   write(connfd, sendBuffer, strlen(sendBuffer));
  
   // Start the communication between client and server 
   processResponse(connfd); 
  
   // Closing the socket and shutting down the server 
   printf("KKJserver shutdown...\n"); 
   close(sockfd); 
}
