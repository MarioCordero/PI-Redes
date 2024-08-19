/*
 *  Esta prueba solo funciona utilizando un equipo de la red interna de la ECCI, por lo que
 *  deberan realizarlo en la ECCI o  conectarse por la VPN para completarla
 *  La direccion IPv6 provista es una direccion privada
 *  Tambien deben prestar atencion al componente que esta luego del "%" en la direccion y que hace
 *  referencia a la interfaz de red utilizada para la conectividad, en el ejemplo se presenta la interfaz "eno1"
 *  pero es posible que su equipo tenga otra interfaz
 *
 */
#include <stdio.h>
#include <string.h>
#include "Socket.h"
#include <iostream>

int main( int argc, char * argv[] ) {
   const char * os = "fe80::194d:4e78:60c:fedc%enp0s31f6";
   const char * request = "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   Socket s( 's', true );                 //Declares a socket on IPv6 mode, look at the constructor -> Socket::Socket( char type, bool IPv6 )
   
   char a[512];                           //This array will allocate 512 bytes of memory

   memset( a, 0, 512 );                   //Sets all 512 bytes of the array a to 0. It initializes the entire array to zero.

   s.Connect( os, (char *) "http" );      //

   s.Write(  request );

   s.Read( a, 512 );
   
   printf( "%s\n", a);

}

