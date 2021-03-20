#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define PORT     56789 
#define BUFFERSIZE 1024 
   
int main( int argc, char *argv[] ) {
    int sockfd, newsockfd, clilen, n;
    char buffer[BUFFERSIZE];
    struct sockaddr_in servaddr, cliaddr;
    char *hello = "Hello from server"; 

    // Creating a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
   
    /* Initialize socket structure */
    bzero(&servaddr, sizeof(servaddr)); 
    bzero(&cliaddr, sizeof(cliaddr)); 
   
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
   
    /* Bind the socket with the server address - This links the 
    *socket to the actual physical address and port number of the 
    *server, in such way that from now on we only need to listen 
    *to the port to receive any incoming server requests.
    */
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
    {
        perror("ERROR on binding");
        exit(1);
    }
      
    /* Now start listening for the clients, here process will
     * go in sleep mode and will wait for the incoming connection
    */
    listen(sockfd,5);
    clilen = sizeof(cliaddr);
   
    // Accept actual connection from the client 
    newsockfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
	
    if (newsockfd < 0) 
    {
        perror("ERROR on accept");
        exit(1);
    }
   
    // If connection is established then start communicating 
    n = recv(newsockfd, buffer, BUFFERSIZE, 0);
    if (n < 0) 
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    buffer[n] = '\0';
    printf("Client : %s\n", buffer); 
   
    // Send a response to the client 
    n = send(newsockfd, hello, strlen(hello), 0);
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    printf("Hello message sent.\n");
    
    close(sockfd);
    return 0;
}