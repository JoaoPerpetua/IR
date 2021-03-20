#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[])
{
	int pid = -1;
	// add here the declarations of variables needed
	int sockfd, portno, n;
    	char buffer[BUFFERSIZE];
    	struct sockaddr_in servaddr;
    	portno = atoi(argv[2]);
	

	// create a TCP socket and connect to the server
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    	if (sockfd  < 0) 
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
   
   	/* Now connect to the server */
    	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) 
    	{
        	perror("ERROR connecting");
        	exit(1);
    	}
    
	while(pid < 0) //Make sure creation of a child process is successfull  
	{
		pid=fork();
	}
	
	while(1==1)
	{
		if (pid==0)
		{
			// Ask user which message he want to send here
			printf("Please enter the message: ");
			bzero(buffer,BUFFERSIZE);
			fgets(buffer,BUFFERSIZE-1,stdin);
		   
			/* Send message to the server */
			n = send(sockfd, buffer, BUFFERSIZE, 0);
			if (n < 0) 
			{
				perror("ERROR writing to socket");
				exit(1);
			}
			printf("Message sent.\n");
		}
		else
		{
			bzero(buffer,BUFFERSIZE);
    			n = recv(sockfd, buffer, BUFFERSIZE-1, 0);
    			if ( n < 0) 
    			{
        			perror("ERROR reading from socket");
        			exit(1);
    			}
    
    			buffer[n] = '\0'; 
    			printf("Server : %s\n", buffer); 
		}
	}

	close(sockfd);

	return 0;

}

