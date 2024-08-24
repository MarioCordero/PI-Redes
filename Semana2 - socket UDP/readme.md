# Semana2 - socket msg UDP IPv4 & IPv6

## Descripción

   **Objetivo**

   - Completar las clases "VSocket" y "Socket" para lograr el intercambio de mensajes utilizando el protocolo UDP en IPv4, para lograr la transferencia de datagramas.

   - Para el intercambio de datagramas no es necesario establecer una conexión.
   - Para este trabajo, será necesario construir un pequeño servidor, quien recibirá los mensajes, identificará el emisor y nos devolverá una tira, y un cliente quién se encargará de enviar el primer datagrama

   **Procedimiento**

   1. Funcionamiento UDP en IPv4

      - Completar la clase "VSocket" para poder intercambiar mensajes UDP

      - Se facilita la interfaz "VSocket.h"

      - Programar el método "int Bind( int )" que se utiliza en el servidor (man bind)

         ```bash
            ...
               host4.sin_family = AF_INET;
               host4.sin_addr.s_addr = htonl( INADDR_ANY );
               host4.sin_port = htons( port );
               memset(host4.sin_zero, '\0', sizeof (host4.sin_zero));
            ...
         ```

      - Programar los métodos: 

         ```bash
            "size_t sendTo( const void *, size_t, void * )"       

            "size_t recvFrom(void *, size_t, void * )"
         ```   
         De manera que los procesos puedan intercambiar mensajes utilizando UDP

      - Archivos facilitados (clase VSocket de las asignaciones anteriores):
         - ipv4-udp-client.cc
         - ipv4-udp-server.cc

   2. Funcionamiento UDP en IPv6
   
      **Objetivo**
         - Completar las clases "VSocket" y "Socket" en C++ para lograr el intercambio de mensajes entre computadoras que no comparten memoria utilizando datagramas con el protocolo UDP en IPv6

      **Tareas**
        - Completar la clase C++ "Socket" para poder intercambiar mensajes entre procesos que no comparten memoria.
        - Se facilita los archivos base para las clases, deben completar la funcionalidad para IPv6:
        - Bind()
        - SentTo()
        - ReceiveFrom()
        - Los ejemplos "ipv6-udp-client.cc" y "ipv6-udp-server.cc" deben funcionar correctamente

   **Ejecución**

- IPv4
   ```bash
   make; make ipv4-udp-server.out

   ./ipv4-udp-server.out #(En una terminal)

   ./ipv4-udp-client.out #(En otra terminal)
   ```

- IPv6
   ```bash
   make; make ipv4-udp-server.out

   ./ipv4-udp-server.out #(En una terminal)

   ./ipv4-udp-client.out #(En otra terminal)
   ```