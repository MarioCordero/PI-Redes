/*
 *  Ejemplo de sockets con IPv4
 *
 */

#include <stdio.h>
#include <string.h>

#include "VSocket.h"
#include "Socket.h"

int main( int argc, char * argv[] ) {
   const char * os = "http://os.ecci.ucr.ac.cr/";
   const char * osi = "10.1.104.187";
   const char * ose = "163.178.104.187";
   const char * request = "GET /lego/ HTTP/1.1\n\rhost: redes.ecci\n\r\n\r";

   VSocket * s;	
   char a[512];

   s = new Socket( 's' );
   s->Connect( osi, 80 );
   s->Write(  request );
   s->Read( a, 512 );
   printf( "%s\n", a);

}