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
   const char * osi = "10.84.166.62";
   const char * ose = "163.178.104.62";
   const char * request = "GET /pirofs/index.php?disk=Disk-01&cmd=ls HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   VSocket * s;	
   char a[512];

   s = new Socket( 's' );  //Creates a Socket class, it's a subclase of VSocket

   s->Connect( osi, 80 );  //Connects the socket to the server using the "osi" IP in the port 80, HTTP standart port

   s->Write(request);      //Sends a request to the server

   s->Read( a, 512 );      //Gets the response of the server and store it on buffer "a" (512 bytes)

   printf( "%s\n", a);     //Prints the response

}