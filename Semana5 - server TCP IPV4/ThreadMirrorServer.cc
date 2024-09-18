/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/

#include <iostream>
#include <thread>

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
	printf("HOLA");
	std::thread * worker;
	VSocket * s1, * client;

	s1 = new Socket( 's' );

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