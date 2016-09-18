#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
/* You will have to modify the program below */

#define MAXBUFSIZE 100

int main (int argc, char * argv[] )
{
	int sock;                               //This will be our socket
	struct sockaddr_in local_addr, remote_addr;     //"Internet socket address structure"
	socklen_t remote_len = sizeof(remote_addr);             //length of the sockaddr_in structure
	int recvlen;                             //number of bytes we receive in our message
	char buffer[MAXBUFSIZE];                //a buffer to store our received message
	
	if (argc != 2)
	{
		printf ("USAGE:  <port>\n");
		exit(1);
	}

	/******************
	  This code populates the sockaddr_in struct with
	  the information about our socket
	 ******************/
	bzero(&local_addr,sizeof(local_addr));                //zero the struct
	local_addr.sin_family = AF_INET;                   //address family
	local_addr.sin_port = htons(atoi(argv[1]));        //htons() sets the port # to network byte order
	local_addr.sin_addr.s_addr = INADDR_ANY;           //supplies the IP address of the local machine


	//Causes the system to create a generic socket of type UDP (datagram)
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("unable to create socket");
	}


	/******************
	  Once we've created a socket, we must bind that socket to the 
	  local address and port we've supplied in the sockaddr_in struct
	 ******************/
	if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0)
	{
		printf("unable to bind socket\n");
	}

	//waits for an incoming message
	bzero(buffer, sizeof(buffer));

	while(1)
	{
		printf("waiting on port %d\n", atoi(argv[1]));
		recvlen = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *)&remote_addr, &remote_len);
	    
	    printf("received %d bytes\n", recvlen);
	    if (recvlen > 0) 
	    {
	        buffer[recvlen] = 0;
	        printf("The client says %s\n", buffer);
	    }

		char msg[] = "Message Received client --sender";
		recvlen = sendto(sock, msg, sizeof(msg), 0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));
	}

	//close(sock);
}

