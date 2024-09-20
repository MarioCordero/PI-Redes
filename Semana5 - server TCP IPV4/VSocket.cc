/**
 *
 *   UCR-ECCI
 *
 *   VSocket class interface
 *
 *   2024-ii
 *
 **/

#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <cstring>			    // memset

#include <sys/socket.h>
#include <arpa/inet.h>			// ntohs
#include <unistd.h>			    // close
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <netdb.h>			    // getaddrinfo, freeaddrinfo
#include <string>

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
void VSocket::CreateVSocket( char t, bool IPv6 ) {
    // First the domain
    int domain = AF_INET; // IPv4 Internet protocols
    if (IPv6) {
        //------------------------------- IPv6 -------------------------------//
        std::cout << "IPv6 confirmed." << std::endl;
        this->IPv6 = true;
        domain = AF_INET6; // IPv6 Internet protocols
    }

    //------------------------------- IPv4 -------------------------------//
    // Then the type
    int type = -1;
    if (t == 's') {             // stream
        type = SOCK_STREAM;
    } else if (t == 'd') {      // datagram
        type = SOCK_DGRAM;
    }

    // Last the protocol
    int protocol = 0;

    // Now, create the socket using the Linux library
    this->idSocket = socket(domain, type, protocol);

    //printf("ID Socket: %d \n" , this->idSocket);
    // Check if there's an error
    if (this->idSocket == -1) {
        std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
        throw std::runtime_error("Problem Creating Vsocket...");
    }
    //std::cout << "TERMINA BIEN el VSocket" << std::endl;
}

//Creates just a socketID
void VSocket::CreateVSocket( int port ){
	this->idSocket = port;
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
void VSocket::Close() {
    //Try to close the socket
    if (close(this->idSocket) == -1) {
        throw std::runtime_error("Error closing socket in Socket::Close()");
    }
}


/**
  * MakeConnection method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dot notation, example "10.84.166.62"
  * @param      int port: process address, example 80
  *
 **/
int VSocket::MakeConnection( const char * hostip, int port ) {

	//The structure of the connect Unix syscall 
	//int connect(int sockfd, const struct sockaddr *addr,socklen_t addrlen);
	//sockfd = sockID
    try{
        int st;

        if(this->IPv6){
            //------------------------------ IPv6 ------------------------------//

            // Para IPv6 
            struct sockaddr_in6  host6;
            struct sockaddr * ha;

            memset( &host6, 0, sizeof( host6 ) );
            host6.sin6_family = AF_INET6;
            st = inet_pton( AF_INET6, hostip, &host6.sin6_addr );
            //printf("ESTOY EN MakeConnection( const char * hostip, int port )");
            if ( st <= 0 ) {	// 0 means invalid address, -1 means address error
                throw std::runtime_error( "Socket::Connect( const char *, int ) [inet_pton]" );
            }

            host6.sin6_port = htons( port );

            ha = (struct sockaddr *) &host6;

            socklen_t len = sizeof( host6 );

            st = connect( this->idSocket, ha, len );
            if ( -1 == st ) {
                char addr_str[INET6_ADDRSTRLEN]; // Buffer para la dirección
                if (inet_ntop(AF_INET6, &host6.sin6_addr, addr_str, sizeof(addr_str)) != NULL) {
                    printf("Dirección IPv6: %s\n", addr_str);
                } else {
                    perror("inet_ntop error");
                }
                printf("Tamaño de host6: %zu bytes\n", (size_t)len);
                printf("ID SOCKET: %d", this->idSocket);
                //printf("Estado de ST: %d \n",st);
                throw std::runtime_error( "Socket::Connect( const char *, int ) [connect]" );
            }

        }else{
            //------------------------------ IPv4 ------------------------------//

            //Define an addres for the host
            //sockaddr_in is a struct on c/c++ that stores an IP and a port
            struct sockaddr_in host4;

            //void *memset(void *ptr, int value, size_t num);
            //initialize the host4 structure in 0
            memset( (char *)&host4 , 0 , sizeof( host4 ) );

            //Type of direction using in the struct host4 (IPv4)
            //sin_family
            //Es un campo de la estructura sockaddr_in que especifica el tipo de direcciones que la estructura puede manejar.
            host4.sin_family = AF_INET;

            //This works jus to prove if we can stablish a connection with AF_INET and the port given
            // Es una función de la biblioteca de sockets en C/C++ que convierte direcciones IP en formato de texto (como "192.168.1.1") a su formato binario en una estructura in_addr o in6_addr, dependiendo de si se está utilizando IPv4 o IPv6.
            // inet_pton significa "Internet Presentation to Numeric".
            st = inet_pton( AF_INET, hostip, &host4.sin_addr );

            if ( -1 == st ) {
                throw(std::runtime_error( "VSocket::DoConnect, inet_pton" ));
            }

            //La línea host4.sin_port = htons(port); se utiliza para asignar el número de puerto al campo sin_port de la estructura sockaddr_in, y convierte el número de puerto al formato de bytes adecuado para la red. Aquí te explico cada parte:

            host4.sin_port = htons( port );

            //Try the connection
            st = connect( idSocket, (sockaddr *) &host4, sizeof( host4 ) );

            if ( -1 == st ) {
                throw(std::runtime_error( "VSocket::DoConnect, connect" ));
            }

            return st;
        }

    }catch(const std::exception& e){

        std::cerr << '\n' << e.what() << '\n';

    }
}

/**
  *
  * @param      host
  * @param      service
  *
 **/
int VSocket::MakeConnection( const char * host, const char * service){


    int st;
    
    struct addrinfo hints, *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));

    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */

    hints.ai_socktype = SOCK_STREAM; /* Stream socket */

    hints.ai_flags = 0;

    hints.ai_protocol = 0;          /* Any protocol */

    st = getaddrinfo( host, service, &hints, &result );

    for (rp = result; rp; rp = rp->ai_next) {
        // Imprimir dirección IP
        char addr_str[INET6_ADDRSTRLEN]; // Buffer para la dirección IP

        if (rp->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *addr = (struct sockaddr_in *)rp->ai_addr;
            inet_ntop(AF_INET, &(addr->sin_addr), addr_str, sizeof(addr_str));
            printf("Conectando a IPv4: %s:%d\n", addr_str, ntohs(addr->sin_port));
        } else if (rp->ai_family == AF_INET6) { // IPv6
            struct sockaddr_in6 *addr = (struct sockaddr_in6 *)rp->ai_addr;
            inet_ntop(AF_INET6, &(addr->sin6_addr), addr_str, sizeof(addr_str));
            printf("Conectando a IPv6: %s:%d\n", addr_str, ntohs(addr->sin6_port));
        }

        st = connect(idSocket, rp->ai_addr, rp->ai_addrlen);
        if (0 == st) {
            break; // Conexión exitosa
        }
    }

    freeaddrinfo( result );
    return st;
}

/**
  * Listen method
  *
  * @param      int queue: max pending connections to enqueue (server mode)
  *
  *  This method define how many elements can wait in queue
  *
 **/
int VSocket::Listen( int queue ) {

    int st = -1;

    st = listen(this->idSocket , queue);

    if ( -1 == st ) {
        throw std::runtime_error( "VSocket::Listen( int )" );
    }

    return st;

}


/**
  * Bind method
  *    use "bind" Unix system call (man 3 bind) (server mode)
  *
  * @param      int port: bind a unamed socket to a port defined in sockaddr structure
  *
  *  Links the calling process to a service at port
  *
 **/
int VSocket::Bind( int port ) {

    int st = -1;

    if(this->IPv6){
        
        struct sockaddr_in6 host6;

        memset(&host6, 0, sizeof(host6)); // Zero out the structure

        host6.sin6_family = AF_INET6; // IPv6

        host6.sin6_addr = in6addr_any; // Bind to any address local or any

        host6.sin6_port = htons(port); // Convert port number to network byte order

        // Imprimir datos de la conexión IPv6
        char addr_str[INET6_ADDRSTRLEN]; // Buffer para la dirección IP
        inet_ntop(AF_INET6, &host6.sin6_addr, addr_str, sizeof(addr_str));
        printf("Binding to IPv6: %s:%d\n", addr_str, ntohs(host6.sin6_port));

        st = bind(idSocket, (const sockaddr *)&host6, sizeof(host6));

    }else{
        struct sockaddr_in host4;

        memset(host4.sin_zero, '\0', sizeof (host4.sin_zero));

        host4.sin_family = AF_INET;

        host4.sin_addr.s_addr = htonl( INADDR_ANY );

        host4.sin_port = htons( port );

        //Imprimir datos
        printf("Binding to IPv4: %s:%d\n", "0.0.0.0", ntohs(host4.sin_port));

        st = bind( idSocket, (const sockaddr *) &host4 , sizeof( host4 ));

    }

    return st;
}

/**
  * DoAccept method
  *    use "accept" Unix system call (man 3 accept) (server mode)
  *
  *  @returns   a new class instance
  *
  *  Waits for a new connection to service (TCP mode: stream)
  *
 **/
int VSocket::DoAccept(){

    int st = -1;

    st = accept(this->idSocket, NULL , NULL);


    if ( -1 == st ) {
        throw std::runtime_error( "VSocket::DoAccept()" );
    }

    return st;
}


/**
  * Shutdown method
  *    use "shutdown" Unix system call (man 3 shutdown)
  *
  *  @param	int mode define how to cease socket operation
  *
  *  Partial close the connection (TCP mode)
  *
 **/
int VSocket::Shutdown( int mode ) {
   int st = -1;

   if ( -1 == st ) {
      throw std::runtime_error( "VSocket::Shutdown( int )" );
   }

   return st;

}


/**
  *  sendTo method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to send data
  *
  *  Send data to another network point (addr) without connection (Datagram)
  *	 [man 2 send]
 **/
size_t VSocket::sendTo( const void * buffer, size_t size, void * addr ) {

   int st = -1;

   struct sockaddr_in *dest_addr = (struct sockaddr_in *) addr;

   socklen_t addrlen = sizeof(*dest_addr);

   st = sendto( idSocket , buffer, size, 0, (struct sockaddr*)dest_addr, addrlen);

   return st;

}


/**
  *  recvFrom method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to receive from data
  *
  *  @return	size_t bytes received
  *
  *  Receive data from another network point (addr) without connection (Datagram)
  *  [man 2 recvfrom]
 **/
size_t VSocket::recvFrom( void * buffer, size_t size, void * addr ) {

   	int st = -1;

	struct sockaddr_in *dest_addr = (struct sockaddr_in *) addr;

	socklen_t addrlen = sizeof(*dest_addr);

	st = recvfrom( idSocket , buffer , size , 0, (struct sockaddr*)dest_addr, &addrlen);

   	return st;
}

/**
 * @brief Procesa una respuesta HTTP para extraer el cuerpo del mensaje.
 * 
 * Esta función toma una respuesta HTTP completa como una cadena y separa el cuerpo del encabezado. El encabezado y el cuerpo están separados por una línea en blanco (`\r\n\r\n`). La función busca esta línea en blanco en la respuesta y devuelve el contenido del cuerpo que sigue a esta separación.
 * 
 * @param[in] response La respuesta HTTP completa en formato de cadena.
 * 
 * @return std::string El cuerpo del mensaje HTTP, es decir, el contenido que sigue a la separación del encabezado.
 * 
 * @throws std::runtime_error Si el formato de la respuesta HTTP no es válido (es decir, si no se encuentra la separación entre el encabezado y el cuerpo).
 */
std::string VSocket::processHttpResponse(const std::string& response) {
    std::string delimiter = "\r\n\r\n";
    size_t pos = response.find(delimiter);
    
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid HTTP response format");
    }
    
    size_t body_start = pos + delimiter.length();
    std::string body = response.substr(body_start);
    
    return body;
}