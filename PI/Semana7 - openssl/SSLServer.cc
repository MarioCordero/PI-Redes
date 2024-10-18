/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/

#include <thread>
#include <cstdlib>	// atoi
#include <cstdio>	// printf
#include <cstring>	// strlen, strcmp

#include "SSLSocket.h"

#define PORT	4321


void Service( SSLSocket * client ) {
	char buf[ 1024 ] = { 0 };
	int sd, bytes;
	const char* ServerResponse="\n<Body>\n\
								\t<Server>os.ecci.ucr.ac.cr</Server>\n\
								\t<dir>ci0123</dir>\n\
								\t<Name>Proyecto Integrador Redes y sistemas Operativos</Name>\n\
								\t<NickName>PIRO</NickName>\n\
								\t<Description>Consolidar e integrar los conocimientos de redes y sistemas operativos</Description>\n\
								\t<Author>profesores PIRO</Author>\n\
								</Body>\n";
	const char *validMessage = "\n<Body>\n\
								\t<UserName>piro</UserName>\n\
								\t<Password>ci0123</Password>\n\
								</Body>\n";
	client->SSLAccept();
	client->SSLShowCerts();

	bytes = client->Read( buf, sizeof( buf ) );
	buf[ bytes ] = '\0';
	printf( "Client msg: \"%s\"\n", buf );

	if ( ! strcmp( validMessage, buf ) ) {
		client->Write( (char *)ServerResponse, strlen( ServerResponse ) );
	} else {
		client->Write( (char *)"Invalid Message", strlen( "Invalid Message" ) );
	}

	client->Close();
}


int main( int cuantos, char ** argumentos ) {
	SSLSocket * server, * client;
	std::thread * worker;
	int port = PORT;

	if ( cuantos > 1 ) {
		port = atoi( argumentos[ 1 ] );
	}

	server = new SSLSocket( 's' );
	server->Bind( port );
	server->Listen( 10 );
	server->SSLInitServer( "ci0123.pem", "ci0123.pem" );

	printf("\nCLIENT %d",client->getSocketDescriptor());
	for( ; ; ) {
		client = dynamic_cast<SSLSocket*>(server->Accept());
		printf("\nCLIENT AFTER CAST %d",client->getSocketDescriptor());
		client->CopyContext( server );
		worker = new std::thread( Service, client );	// service connection
	}
}