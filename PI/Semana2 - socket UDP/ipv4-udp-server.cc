/**
  *  Server-side implementation of UDP client-server model	
  *
  *  CI-0123 Proyecto integrador de redes y sistemas operativos
  *
 **/

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "VSocket.h" 
#include "Socket.h" 

#define PORT	 6789 
#define MAXLINE 1024 

int main() {

	VSocket * server;
	int len, n;
	int sockfd;
	struct sockaddr other; // struct sockaddr is used for compatibility with functions that need a generic address type, while struct sockaddr_in is used for specific handling of IPv4 addresses.
	char buffer[MAXLINE];
	char *hello = (char *) "Hello from CI0123 server";

	server = new Socket( 'd', false ); // Create a socket in IPv4

	server->Bind( PORT ); // bind = link, binds the UDP socket to the specified port number (6789).

	memset( &other, 0, sizeof( other ) ); // Set the Struct sockaddr in 0, empty

	n = server->recvFrom( (void *) buffer, MAXLINE, (void *) &other );	// Mensaje de los www servers

	buffer[n] = '\0'; // Adds a null char at the end of the string

	printf("Server: message received: %s\n", buffer);

	server->sendTo( (const void *) hello, strlen( hello ), (void *) &other );

	printf("Server: Hello message sent.\n");

	server->Close();

	return 0;
}

