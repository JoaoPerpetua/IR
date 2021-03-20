#include <stdio.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <unistd.h>
   
#define BUFFERSIZE 1024 
   
int main(int argc, char *argv[]) 
{ 
    int sockfd,portno, n, len; 
    char buffer[BUFFERSIZE];
    struct sockaddr_in servaddr;

    if (argc < 3) 
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
  
    /* Creating an UDP socket */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
    if (sockfd < 0) 
    { 
        perror("ERROR opening socket");
        exit(1); 
    } 
  
    // Emptying servaddr struct
    bzero(&servaddr, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(portno); 
    inet_pton(AF_INET,argv[1], &(servaddr.sin_addr)); 
    

    /* Now ask for a message from the user, this message
     * will be read by server
    */
    printf("Please enter the message for the server: ");
    bzero(buffer,BUFFERSIZE);
    fgets(buffer,BUFFERSIZE-1,stdin);

    
    /* Send message to the server */
    n = sendto(sockfd, buffer, BUFFERSIZE, MSG_CONFIRM, (const struct sockaddr *) &servaddr,  sizeof(servaddr));
    if (n < 0) 
    {
        perror("ERROR writing to socket");
        exit(1);
    }
    printf("Message sent.\n"); 

    /* Now read server response */
    bzero(buffer,BUFFERSIZE);
    n = recvfrom(sockfd, (char *)buffer, BUFFERSIZE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    if(n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    
    buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
} 