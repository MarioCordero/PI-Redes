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
   const char * lab = "fe80::4161:e292:8c1d:e3c0%enp0s31f6";
   const char * request = "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   Socket s( 's', true );                 //Declares a socket on IPv6 mode, look at the constructor -> Socket::Socket( char type, bool IPv6 )
   
   char a[512];                           //This array will allocate 512 bytes of memory

   memset( a, 0, 512 );                   //Sets all 512 bytes of the array a to 0. It initializes the entire array to zero.


   s.Connect( lab, (char *) "http" );      //

   int condition = 1;

   while (condition != 0){

      s.Write(  request );

      condition = s.Read( a, 512 );
      
      printf( "%s\n", a);

   }

   return 0;
}

