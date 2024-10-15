/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/

#include <iostream>
#include <thread>
#include <string.h>	// memset

#include "Socket.h"

#define PORT 5678
#define BUFSIZE 512


/**
 *   Task each new thread will run
 *      Read string from socket
 *      Write it back to client
 *
 **/
void task( VSocket * client ) {
	char a[ BUFSIZE ];

	client->Read( a, BUFSIZE );	// Read a string from client, data will be limited by BUFSIZE bytes
	std::cout << "Server received: " << a << std::endl;
	client->Write( a );		// Write it back to client, this is the mirror function
	client->Close();		// Close socket in parent
}


/**
 *   Create server code
 *      Infinite for
 *         Wait for client conection
 *         Spawn a new thread to handle client request
 *
 **/
int main( int argc, char ** argv ) {
	std::thread * worker;
	VSocket * s1, * client;

	bool isIPv6 = true;  // Default IPv6

	// Verify the argument to choose IPv4 o IPv6
	if (argc > 1) {
		if (strcmp(argv[1], "ipv4") == 0) {

			isIPv6 = false;  // Use IPv4
			s1 = new Socket('s'); // Create a socket IPv4
		
		} else if (strcmp(argv[1], "ipv6") == 0) {
		
			isIPv6 = true;  // Use IPv6
			s1 = new Socket('s', "5678"); //Create a socket IPv6
		
		} else {
		
			fprintf(stderr, "Uso: %s [ipv4|ipv6]\n", argv[0]);
			exit(1);
		
		}
	}else{
		fprintf(stderr, "Uso: %s [ipv4|ipv6]\n", argv[0]);
		exit(1);
	}

	if (!s1) {
		perror("Failed to create socket");
		exit(1);
	}

	// Port to access this mirror server
	if (s1->Bind(PORT) < 0) {
		perror("Bind failed");
		exit(1);
   	}
	
	s1->Listen( 5 );		// Set backlog queue to 5 connections

	for( ; ; ) {
		client = s1->Accept();	 	// Wait for a client connection
		worker = new std::thread( task, client );
	}
}