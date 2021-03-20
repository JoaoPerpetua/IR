#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>
   
#define PORT     56789 
#define BUFFERSIZE 1024 
  
// Driver code 
int main(int argc, char *argv[]) { 
    int sockfd, newsockfd, clilen, n;
    char buffer[BUFFERSIZE];
    struct sockaddr_in servaddr, cliaddr;
    char *hello = "Hello from server";  
      
    // Creating an UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }
      
    bzero(&servaddr, sizeof(servaddr)); 
    bzero(&cliaddr, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT); 
      
    /* Bind the socket with the server address - This links the 
     *socket to the actual physical address and port number of the 
     *server, in such way that from now on we are ready to receive 
     *any incoming server requests.
     */
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) 
    { 
        perror("ERROR on binding");
        exit(1);
    } 
      
    n = recvfrom(sockfd, (char *)buffer, BUFFERSIZE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &clilen); 
    if(n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    buffer[n] = '\0'; 
    printf("Client : %s\n", buffer); 

    // Send a response to the client
    n = sendto(sockfd, hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, sizeof(cliaddr)); 
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    printf("Hello message sent.\n");
    
    close(sockfd);
    return 0;
} 
