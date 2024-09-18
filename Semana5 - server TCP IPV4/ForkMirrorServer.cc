/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>	// memset
#include <unistd.h>

#include "Socket.h"

#define PORT 5678
#define BUFSIZE 512

int main( int argc, char ** argv ) {
	VSocket * s1, * s2;
	int childpid;
	char a[ BUFSIZE ];

	s1 = new Socket('s');
	if (!s1) {
		perror("Failed to create socket");
		exit(1);
	}

	if (s1->Bind(PORT) < 0) {
		perror("Bind failed");
		exit(1);
	}

	s1->Listen( 5 );			// Set backlog queue to 5 connections

	for( ; ; ) {
		s2 = s1->Accept();	 		// Wait for a new connection, connection info is in s2 variable
		childpid = fork();		// Create a child to serve the request
		if ( childpid < 0 ) {
			perror( "server: fork error" );
		} else {
			if (0 == childpid) {		// child code
			s1->Close();			// Close original socket "s1" in child
			memset( a, 0, BUFSIZE );
			s2->Read( a, BUFSIZE );	// Read a string from client using new conection info
			s2->Write( a );		// Write it back to client, this is the mirror function
			exit( 0 );			// Exit, finish child work
			}
		}
		s2->Close();			// Close socket s2 in parent, then go wait for a new conection
	}
}