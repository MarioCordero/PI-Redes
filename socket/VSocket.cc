/**
 *  Establece la definición de la clase Socket para efectuar la comunicación
 *  de procesos que no comparten memoria, utilizando un esquema de memoria
 *  distribuida.  El desarrollo de esta clase se hará en varias etapas, primero
 *  los métodos necesarios para los clientes, en la otras etapas los métodos para el servidor,
 *  manejo de IP-v6, conexiones seguras y otros
 *
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2024-iI
 *  Grupo: 3
 *
 * (versión Fedora)
 *
 **/

#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <cstring>			// memset

#include <sys/socket.h>
#include <arpa/inet.h>			// ntohs
#include <unistd.h>			// close
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>			// getaddrinfo, freeaddrinfo

#include "VSocket.h"


/**
  *  Class initializer
  *     use Unix socket system call
  *     Creates an initializes a socket in a network
  * 
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
void VSocket::CreateVSocket( char t, bool IPv6 ){
	//The structure of the Unix socket syscall 
    //int socket(int domain, int type, int protocol);

	//First the domain
    int domain = AF_INET; //IPv4 Internet protocols
    if(IPv6){
        domain = AF_INET6; //IPv6 Internet protocols
    }

	//Then the type
    int type = -1;
    if(t=='s'){             //stream
        type = SOCK_STREAM;
    }else if(t=='d'){       //datagram
        type = SOCK_DGRAM;
    }

	//Last the protocol
    int protocol = 0;

	//Now, create the socket using the linux library
	this->idSocket = socket(domain, type, 0);

	//Check if there's an error
    if (this->idSocket == -1) {
        perror("Error!!!!");
    }
}

//Creates just a socketID
void VSocket::CreateVSocket( int port ){
	this->idSocket = port;
}


/**
  * Class destructor
  *
 **/
VSocket::~VSocket() {

   this->Close();

}


/**
  * Close method
  *    use Unix close system call (once opened a socket is managed like a file in Unix)
  *
 **/
void VSocket::Close() {
    //Try to close the socket
    if (close(this->idSocket) == -1) {
        throw std::runtime_error("Error closing socket in Socket::Close()");
    }
}


/**
  * DoConnect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dot notation, example "10.1.104.187"
  * @param      int port: process address, example 80
  *
 **/
int VSocket::MakeConnection( const char * hostip, int port ) {

	//The structure of the connect Unix syscall 
	//int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	//sockfd = sockID
   int st;

   if ( -1 == st ) {
      perror( "VSocket::MakeConnection" );
      throw std::runtime_error( "VSocket::MakeConnection" );
   }

   return st;

}

