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
#include <iostream>

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

        //------------------------------- IPv6 -------------------------------//
        std::cout<<"IPV6 confirmed.";
        domain = AF_INET6; //IPv6 Internet protocols

    }
        
    //------------------------------- IPv4 -------------------------------//
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
    this->idSocket = socket(domain, type, protocol);

    //Check if there's an error
    if (this->idSocket == -1) {
        throw(std::runtime_error("Problem Crating Vsocket..."));
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
    try{
        int st;

        if(this->IPv6){
            //------------------------------ IPv6 ------------------------------//

            // Para IPv6 
            // struct sockaddr_in6  host6;
            // struct sockaddr * ha;

            // memset( &host6, 0, sizeof( host6 ) );
            // host6.sin6_family = AF_INET6;
            // st = inet_pton( AF_INET6, hostip, &host6.sin6_addr );
            // if ( 0 <= st ) {	// 0 means invalid address, -1 means address error
            //    throw std::runtime_error( "Socket::Connect( const char *, int ) [inet_pton]" );
            // }
            // host6.sin6_port = htons( port );
            // ha = (struct sockaddr *) &host6;
            // len = sizeof( host6 );
            // st = connect( this->id, ha, len );
            // if ( -1 == st ) {
            //    throw std::runtime_error( "Socket::Connect( const char *, int ) [connect]" );
            // }

        }else{
            //------------------------------ IPv4 ------------------------------//

            //Define an addres for the host
            //sockaddr_in is a struct on c/c++ that stores an IP and a port
            struct sockaddr_in host4;

            //void *memset(void *ptr, int value, size_t num);
            //initialize the host4 structure in 0
            memset( (char *)&host4 , 0 , sizeof( host4 ) );

            //Type of direction using in the struct host4 (IPv4)
            //sin_family
            //Es un campo de la estructura sockaddr_in que especifica el tipo de direcciones que la estructura puede manejar.
            host4.sin_family = AF_INET;

            //This works jus to prove if we can stablish a connection with AF_INET and the port given
            // Es una función de la biblioteca de sockets en C/C++ que convierte direcciones IP en formato de texto (como "192.168.1.1") a su formato binario en una estructura in_addr o in6_addr, dependiendo de si se está utilizando IPv4 o IPv6.
            // inet_pton significa "Internet Presentation to Numeric".
            st = inet_pton( AF_INET, hostip, &host4.sin_addr );

            if ( -1 == st ) {
                throw(std::runtime_error( "VSocket::DoConnect, inet_pton" ));
            }

            //La línea host4.sin_port = htons(port); se utiliza para asignar el número de puerto al campo sin_port de la estructura sockaddr_in, y convierte el número de puerto al formato de bytes adecuado para la red. Aquí te explico cada parte:

            host4.sin_port = htons( port );

            //Try the connection
            st = connect( idSocket, (sockaddr *) &host4, sizeof( host4 ) );

            if ( -1 == st ) {
                throw(std::runtime_error( "VSocket::DoConnect, connect" ));
            }

            return st;
        }

    }catch(const std::exception& e){

        std::cerr << '\n' << e.what() << '\n';

    }
}

/**
  *
  * @param      host
  * @param      service
  *
 **/
int VSocket::MakeConnection( const char * host, const char * service){

    int st;
    
    struct addrinfo hints, *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */

    hints.ai_socktype = SOCK_STREAM; /* Stream socket */

    hints.ai_flags = 0;

    hints.ai_protocol = 0;          /* Any protocol */

    st = getaddrinfo( host, service, &hints, &result );

    for ( rp = result; rp; rp = rp->ai_next ) {
        st = connect( idSocket, rp->ai_addr, rp->ai_addrlen );
        if ( 0 == st )
        break;
    }

    freeaddrinfo( result );
}