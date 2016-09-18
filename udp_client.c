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
#include <errno.h>

#define MAXBUFSIZE 100

/* You will have to modify the program below */

int main (int argc, char * argv[])
{
	int nbytes;                             // number of bytes send by sendto()
	int sock;                               //this will be our socket
	char buffer[MAXBUFSIZE];

	struct sockaddr_in remote_addr;              //"Internet socket address structure"

	if (argc < 3)
	{
		printf("USAGE:  <server_ip> <server_port>\n");
		exit(1);
	}

	/******************
	  Here we populate a sockaddr_in struct with
	  information regarding where we'd like to send our packet 
	  i.e the Server.
	 ******************/
	bzero(&remote_addr,sizeof(remote_addr));               //zero the struct
	remote_addr.sin_family = AF_INET;                 //address family
	remote_addr.sin_port = htons(atoi(argv[2]));      //sets port to network byte order
	remote_addr.sin_addr.s_addr = inet_addr(argv[1]); //sets remote_addr IP address

	//Causes the system to create a generic socket of type UDP (datagram)
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("unable to create socket");
	}

	/******************
	  sendto() sends immediately.  
	  it will report an error if the message fails to leave the computer
	  however, with UDP, there is no error if the message is lost in the network once it leaves the computer.
	 ******************/
	char command[] = "Taylor is beautiful and I love her very much";
	nbytes = sendto(sock, command, sizeof(command), 0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));

	// Blocks till bytes are received
	struct sockaddr_in from_addr;
	socklen_t from_len = sizeof(remote_addr);
	bzero(buffer,sizeof(buffer));
	nbytes = recvfrom(sock, buffer, MAXBUFSIZE, 0, (struct sockaddr *) &from_addr, &from_len);

	printf("Server says: \'%s\'\n", buffer);

	close(sock);

}

