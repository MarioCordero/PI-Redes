/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2024-ii
  *  Grupo: 3 y 5
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
#include <unistd.h>     //
#include <iostream>

#include "Socket.h"
#include <stdexcept>

// Default constructor implementation
Socket::Socket() : Socket('s', false) {
    // Calls the stream (TCP) IPv4 constructor as default
}

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
  *  Class constructor
  *
  *  @param     int id: socket descriptor
  *
 **/
Socket::Socket( int id ) {

   this->CreateVSocket( id );

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
   printf("PORT: %d \n", port);
   return this->MakeConnection( host, port );

}

int Socket::Connect( const char * server, const char * service ) {

   //printf("Entra al connect IPv6. \n");
   return this->MakeConnection( server, service );
   
}

int Socket::getSocketDescriptor() const {
   return this->idSocket;
}


/**
  * Read method
  *   use "read" Unix system call (man 3 read)
  *
  * @param	void * text: buffer to store data read from socket
  * @param	int size: buffer capacity, read will stop if buffer is full
  *
 **/
size_t Socket::Read(void *text, size_t size) {
   int st = -1;

   // // Cast the void* to char* to print the content as characters
   // char* charText = static_cast<char*>(text);

   // // Print size and the first character (before reading)
   // printf("Buffer size: %zu, First char before reading: %c\n", size, charText[0]);

   st = read(this->idSocket, text, size);

   if (st == -1) {
      throw std::runtime_error("Socket::Read( const void *, size_t )");
   }

   // Print what was read (if any)
   // printf("Data read: %.*s\n", st, charText); // Use %.*s to print the exact number of bytes read

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
size_t Socket::Write(void *text, size_t size) {
   int st = -1;

   // // Cast the void* to char* to print the content as characters
   // char* charText = static_cast<char*>(text);

   // // Print the buffer and size before writing
   // printf("Buffer size: %zu, Data to write: %.*s\n", size, (int)size, charText);

   st = write(this->idSocket, text, size);

   if (st == -1) {
      throw std::runtime_error("Socket::Write( void *, size_t )");
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
size_t Socket::Write(char *text) {
   int st = -1;

   // Use the other Write method
   st = Write(text, strlen(text));

   if (st == -1) {
      throw std::runtime_error("Socket::Write( void *, size_t )");
   }

   return st;
}

/**
  * Accept method
  *    use base class to accept connections
  *
  *  @returns   a new class instance
  *
  *  Waits for a new connection to service (TCP mode: stream)
  *
 **/
Socket * Socket::Accept(){
   int id;
   Socket * other;

   id = this->DoAccept();

   other = new Socket( id );

   return other;

}