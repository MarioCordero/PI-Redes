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
//#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>			// getaddrinfo, freeaddrinfo

#include "VSocket.h"


/**
  *  Class initializer
  *     use Unix socket system call
  *
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
void VSocket::CreateVSocket( char t, bool IPv6 ){

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
void VSocket::Close(){
   int st;

   if ( -1 == st ) {
      throw std::runtime_error( "Socket::Close()" );
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
   
   int st;

   if ( -1 == st ) {
      perror( "VSocket::MakeConnection" );
      throw std::runtime_error( "VSocket::MakeConnection" );
   }

   return st;

}

