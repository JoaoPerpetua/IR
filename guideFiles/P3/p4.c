#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <arpa/inet.h> //close 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 
	
#define PORT 56789
#define BUFFERSIZE 1024
#define SOCKSIZE 100
#define TRUE 1
#define FALSE 0 

int main( int argc, char *argv[] ) {
	int opt = TRUE;
	static fd_set rfds;
	static int retval;
	static int max;
	int client_socket[30];
	int n_clients = 30;
	int master_socket, sd,max_sd,new_socket,valread;
	char buffer[1025];
	int addrlen;
	char msg[] = "Welcome to the server, your Host ID is ";
	struct timeval timeout;
	struct sockaddr_in address;
	int poke = 10;
	static int sockfd;
	char aux[1];
	// add here the declarations of variables needed
	// create a TCP socket and connect to the server
	int i;
	//initialise all client_socket[] to 0 so not checked 
	for (i = 0; i < sizeof(client_socket); i++) 
	{ 
		client_socket[i] = 0; 
	} 
	
	//create a master socket 
	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}
	sockfd = master_socket;
	// set master socket to allow mutiple connections
	if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, 
		sizeof(opt)) < 0 ) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	}
	//type of socket created 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT );
	
	//bind the socket to localhost port 56789
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Listener on port %d \n", PORT);
	// 
	listen(master_socket,5);
	max=master_socket;
	timeout.tv_sec = poke;
	timeout.tv_usec = 0;
	
	while(1){
		// clear the socket set
		FD_ZERO(&rfds);
		// add master socket to set
		FD_SET(master_socket, &rfds);
		// add child sockets to set
		for ( i = 0 ; i < sizeof(client_socket) ; i++) 
		{ 
			//socket descriptor 
			sd = client_socket[i]; 
				
			//if valid socket descriptor then add to read list 
			if(sd > 0) 
				FD_SET( sd , &rfds); 
				
			//highest file descriptor number, need it for the select function 
			if(sd > max_sd) 
				max_sd = sd; 
		} 
		// add other sockets to descriptor set
		//wait for an activity on one of the sockets , timeout is NULL , 
		//so wait indefinitely 
		retval=select(max+1,&rfds,NULL,NULL,&timeout);
		if (retval < 0)
		{
			perror("ERROR on select");
			exit(1);
		}
		if(retval==0)
		{
			timeout.tv_sec = poke;
			timeout.tv_usec = 0;
			// send periodic message
		}
		else
		{
		if(FD_ISSET(master_socket, &rfds)){
		// accept a new connection
			if ((new_socket = accept(master_socket, 
					(struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
			{ 
				perror("accept"); 
				exit(EXIT_FAILURE);
			}
			//add new socket to array of sockets 
			for (i = 0; i < sizeof(client_socket); i++) 
			{
			
				//if position is empty 
				if( client_socket[i] == 0 ) 
				{ 
					client_socket[i] = new_socket; 
					printf("Adding to list of sockets as %d\n" , i);
					sprintf(aux,"%d",i);
					
					
					send(new_socket, msg, strlen(msg), 0);
					break; 
				} 
			}
			
		}
		
		// verify if any other socket received any message
		for (i = 0; i < sizeof(client_socket); i++) 
		{ 
			sd = client_socket[i]; 
				
			if (FD_ISSET( sd , &rfds)) 
			{ 
				//Check if it was for closing , and also read the 
				//incoming message 
				if ((valread = read( sd , buffer, 1024)) == 0) 
				{ 
					//Somebody disconnected , get his details and print 
					getpeername(sd , (struct sockaddr*)&address , 
						(socklen_t*)&addrlen); 
					printf("Host disconnected , ip %s , port %d \n" , 
						inet_ntoa(address.sin_addr) , ntohs(address.sin_port)); 
						
					//Close the socket and mark as 0 in list for reuse 
					close( sd ); 
					client_socket[i] = 0; 
				} 
					
				//Echo back the message that came in 
				else
				{ 
					//set the string terminating NULL byte on the end 
					//of the data read 
					buffer[valread] = '\0'; 
					send(sd , buffer , strlen(buffer) , 0 ); 
				} 
			} 
		}
		
		}
	}
	close(sockfd);
	return 0;
}
