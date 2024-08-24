/**
  *
  *   Client side implementation of UDP client-server model 
  *
  *   CI-0123 Proyecto integrador de redes y sistemas operativos
  *
 **/

#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "VSocket.h"
#include "Socket.h"
#include <cstdlib>


#define PORT    6789 
#define MAXLINE 1024 

int main() {
	VSocket * client; //	Creates a Vsocket

	int sockfd;
	int n, len;
	char buffer[MAXLINE];
	char *hello = (char *) "Hello from CI0123 client";

	struct sockaddr_in other; // Specific for IPv4 connections

	client = new Socket( 'd' );	// Creates an UDP socket: datagram "d"

	memset( &other, 0, sizeof( other ) ); // Set the Struct sockaddr_in in 0, empty

	other.sin_family = AF_INET; // Directions Family IPv4

	other.sin_port = htons(PORT); //sin_port it's an atribute that stores the number of port in network format, htons means "Host TO Network Short".
	
	// 192.168.28.212 -> my apartment IP
	n = inet_pton( AF_INET, "192.168.28.212", &other.sin_addr );	// IP address to test our client with a Python server on lab 3-5

	if ( 1 != n ) {
		printf( "Error converting from IP address\n" );
		exit( 23 ); // Error to convert IP code -> "23"
	}

	n = client->sendTo( (void *) hello, strlen( hello ), (void *) & other ); // sendTo Method that we have to do

	printf("Client: Hello message sent.\n");

	n = client->recvFrom( (void *) buffer, MAXLINE, (void *) & other ); // recvFrom Method that we have to do

	buffer[n] = '\0'; // Adds a null char at the end of the string

	printf("Client message received: %s\n", buffer);

	client->Close();

	return 0;
}