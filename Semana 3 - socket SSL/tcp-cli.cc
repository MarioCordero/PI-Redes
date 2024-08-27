/**
  *  Para IPv6 esta prueba solo funciona utilizando un equipo de la red interna de la ECCI,
  *  por lo que deberán realizarlo en la ECCI o conectarse por la VPN para completarla
  *  La dirección IPv6 provista es una dirección privada
  *
  *  También deben prestar atención al componente que esta luego del "%" en la dirección y que hace
  *  referencia a la interfaz de red utilizada para la conectividad: "enp0s31f6"
  *
  *  Puede obtener las direcciones de sus tarjetas de red con el comando "ip addr"
  *
  *  Uso:
  *
  *    ./tcp-cli.out ipVer[4|6] SSL[0|1]
  *
 **/
 
 #include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "VSocket.h"
#include "Socket.h"
#include "SSLSocket.h"

int main( int argc, char * argv[] ) {
   const char * osv6 = "fe80::194d:4e78:60c:fedc%eno1";
   const char * osv4 = "10.1.104.187";
   const char * request = "GET / HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";
   VSocket * client;
   char a[512];
   int ipVer = 4;	// 4 = IPv4, 6 = IPv6
   int ssl = 0;		// 0 = non SSL, 1 = SSL

   memset( a, 0, 512 );

   if ( argc > 2 ) {
      ipVer = atoi( argv[ 1 ] );
      ssl = atoi( argv[ 2 ] );
   } else {
      if ( argc > 1 ) {
         ipVer = atoi( argv[ 1 ] );
      }
   }

   if ( ipVer != 6 ) {
      ipVer = 4;
   }
   if ( ssl != 1 ) {
      ssl = 0;
   }

   if ( ssl ) {
      printf( "Connecting SSL" );
      if ( 4 == ipVer ) {
         client = new SSLSocket();			// Create an IPv4 TCP SSL socket
         client->Connect( osv4, 443 );
         printf( " IPv4\n" );
      } else {
         client = new SSLSocket( true );	// Create an IPv6 TCP SSL socket
         client->Connect( osv6, "https" );
         printf( " IPv6\n" );
      }
   } else {							// Non SSL socket
      printf( "Connecting non-SSL" );
      if ( 4 == ipVer ) {
         client = new Socket( 's' );		// Create an IPv4 TCP socket
         client->Connect( osv4, 80 );
         printf( " IPv4\n" );
      } else {
         client = new Socket( 's', true );	// Create an IPv6 TCP socket
         client->Connect( osv6, "http" );
         printf( " IPv6\n" );
      }
   }

   client->Write(  request );
   client->Read( a, 511 );
   printf( "%s\n", a);

}