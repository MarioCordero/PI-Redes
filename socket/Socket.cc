/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2024-ii
  *  Grupo: 3
  *
  *   Clase para programar con los "sockets" en Linux
  *
 **/

#include <stdio.h>	// for perror
#include <stdlib.h>	// for exit
#include <string.h>	// for memset
#include <arpa/inet.h>	// for inet_pton
#include <sys/types.h>	// for connect 
#include <sys/socket.h>

#include "Socket.h"

/**
  *  Class constructor
  *     use Unix socket system call
  *
  *  @param	char type: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param	bool ipv6: if we need a IPv6 socket
  *
 **/
Socket::Socket( char type, bool IPv6 ){

   this->CreateVSocket( type, IPv6 );

}


/**
  * Class destructor
  *
 **/
Socket::~Socket(){
}


/**
  * Connect method
  *   use "connect" Unix system call
  *
  * @param	char * host: host address in dot notation, example "10.1.104.187"
  * @param	int port: process address, example 80
  *
 **/
int Socket::Connect( const char * host, int port ) {

   return this->MakeConnection( host, port );

}


/**
  * Read method
  *   use "read" Unix system call (man 3 read)
  *
  * @param	void * text: buffer to store data read from socket
  * @param	int size: buffer capacity, read will stop if buffer is full
  *
 **/
size_t Socket::Read( const void * text, size_t size ) {
   int st = -1;

   if ( -1 == st ) {
//      throw std::runtime_error( "Socket::Read( const void *, size_t )" );
   }

   return st;

}


/**
  * Write method
  *   use "write" Unix system call (man 3 write)
  *
  * @param	void * buffer: buffer to store data write to socket
  * @param	size_t size: buffer capacity, number of bytes to write
  *
 **/
size_t Socket::Write( const void *text, size_t size ) {
   int st = -1;

   if ( -1 == st ) {
//      throw std::runtime_error( "Socket::Write( void *, size_t )" );
   }

   return st;

}


/**
  * Write method
  *
  * @param	char * text: string to store data write to socket
  *
  *  This method write a string to socket, use strlen to determine how many bytes
  *
 **/
size_t Socket::Write( const char *text ) {
   int st = -1;

   if ( -1 == st ) {
//      throw std::runtime_error( "Socket::Write( void *, size_t )" );
   }

   return st;

}

