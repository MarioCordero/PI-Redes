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
 *  2024-ii
 *  Grupo: 3 y 5
 *
 *
 **/

#ifndef Socket_h
#define Socket_h

#include "VSocket.h"

class Socket : public VSocket {

   public:
      Socket( char, bool = false );
      Socket( int );
      ~Socket();
      int Connect( const char *, int );
      int Connect( const char *, const char * );
      size_t Read( void *, size_t );
      size_t Write( void *, size_t );
      size_t Write( char * );
      Socket * Accept();

   protected:

};

#endif