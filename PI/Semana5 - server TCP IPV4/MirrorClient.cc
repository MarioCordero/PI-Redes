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


	bool isIPv6 = true;  // Default IPv6

	//La IP de mi casa :D 	IPv4 192.168.0.8
	//La IP de mi casa :D 	IPv6 fe80::ac5:edd1:a51d:e7a9%eno1
		// Tener en cuenta que para el IPv6 debe correr `ip address` para verificar su direccion IPv6 y la interfaz usada, en mi caso es la configuracion anteriormente mostrada
	
	char * IPv4 = "192.168.0.8";
	char * IPv6 = "fe80::ac5:edd1:a51d:e7a9%eno1";

	// Verify the argument to choose IPv4 o IPv6
	if (argc > 1) {
		if (strcmp(argv[1], "ipv4") == 0) {

			isIPv6 = false;  // Use IPv4
			try {
				s = new Socket('s'); // Create a socket IPv4
			} catch (const std::runtime_error &e) {
				fprintf(stderr, "Error creating socket: %s\n", e.what());
				return -1;
			}

			memset(buffer, 0, BUFSIZE);

			// Connect to the server
			try {
				s->Connect( IPv4 , PORT);
			} catch (const std::runtime_error &e) {
				fprintf(stderr, "Error connecting to server: %s\n", e.what());
				delete s;
				return -1;
			}

		} else if (strcmp(argv[1], "ipv6") == 0) {
		
			isIPv6 = true;  // Use IPv6
			try {
				s = new Socket('s', "5678"); //Create a socket IPv6
			} catch (const std::runtime_error &e) {
				fprintf(stderr, "Error creating socket: %s\n", e.what());
				return -1;
			}

			memset(buffer, 0, BUFSIZE);

			try {
				s->Connect( IPv6 , "5678");  // Same port as server (IPv6), use "" on the second param
			} catch (const std::runtime_error &e) {
				fprintf(stderr, "Error connecting to server: %s\n", e.what());
				delete s;
				return -1;
			}
		
		} else {
		
			fprintf(stderr, "Uso: %s [ipv4|ipv6]\n", argv[0]);
			exit(1);
		
		}
	}else{
		fprintf(stderr, "Uso: %s [ipv4|ipv6]\n", argv[0]);
		exit(1);
	}

	// Send message to server
	try {
		if (argc > 2) {
			s->Write(argv[2]);  // Send second program argument to server
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