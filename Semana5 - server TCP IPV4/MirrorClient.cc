/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example
 *
 *   Deben determinar la dirección IP del equipo donde van a correr el servidor
 *   para hacer la conexión en ese punto (ip addr)
 *
 **/

#include <stdio.h>
#include <cstring>
#include "Socket.h"
#include <stdexcept>

#define PORT 5678
#define BUFSIZE 512

int main( int argc, char **argv ) {
	VSocket *s;
	char buffer[BUFSIZE];

	// Create a new stream IPv4 socket
	try {
		s = new Socket('s');  
	} catch (const std::runtime_error &e) {
		fprintf(stderr, "Error creating socket: %s\n", e.what());
		return -1;
	}

	// Zero-fill buffer
	memset(buffer, 0, BUFSIZE);

	// Connect to the server
	try {
		//La IP de mi casa :D
		s->Connect("192.168.0.8", PORT);  // Same port as server
	} catch (const std::runtime_error &e) {
		fprintf(stderr, "Error connecting to server: %s\n", e.what());
		delete s;
		return -1;
	}

	// Send message to server
	try {
		if (argc > 1) {
			s->Write(argv[1]);  // Send first program argument to server
		} else {
			s->Write("Hello world 2024 ...");
		}
	} catch (const std::runtime_error &e) {
		fprintf(stderr, "Error writing to server: %s\n", e.what());
		delete s;
		return -1;
	}

	// Read answer sent back from server
	try {
		s->Read(buffer, BUFSIZE);  // Read the message from server
		printf("%s\n", buffer);  // Print received string
	} catch (const std::runtime_error &e) {
		fprintf(stderr, "Error reading from server: %s\n", e.what());
		delete s;
		return -1;
	}

	// Clean up and free memory
	delete s;
	return 0;

}
