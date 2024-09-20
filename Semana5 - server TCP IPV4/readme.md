# Semana5 - server TCP IPV4 Trabajo en clase
  - ## Descripción 
 
    **Objetivos**

    -   Completar la clase para intercambio de mensajes entre computadoras que no comparten memoria (TCP IPv4)
    -   Agregar la funcionalidad del servidor

    **Introducción**

    -   Vamos a construir un servidor que sea capaz de atender cada solicitud por medio de un proceso separado (fork)
    -   Vamos a completar la funcionalidad con hilos también

    **Funcionamiento**

    -   En esta entrega, vamos a crear los métodos de una clase "Socket" para lograr que un proceso servidor pueda atender solicitudes de los clientes.  Para que ambos procesos puedan "comunicarse" va a ser necesario definir un protocolo que ambos entiendan.  Para este trabajo en clase, este protocolo será muy sencillo.

    -   Paso de mensajes por medio de sistemas Linux, descripción lógica

        ```bash
        ________________                                ________________
        |                |                              |                |
        |   -----------  |                              |   -----------  |
        |  |           | |                              |  |           | |
        |  | Proceso C | |                              |  | Proceso S | |
        |  |           | |                              |  |           | |
        |   -----------  |                              |   -----------  |
        |                |                              |                |
        |  ============  |                              |  ============  |
        |  +          +  |         /~~~~~~~~~~\         |  +          +  |
        |  + Sist. Op.+  |         |  Red de  |         |  + Sist. Op.+  |
        |  +  (Linux) +  |<------->|  comunic.|<------->|  + (Linux)  +  |
        |  ============  |         \~~~~~~~~~~/         |  ============  |
        |   Máquina A    |                              |   Máquina B    |
        |________________|                              |________________|
        ```
    
    **Tareas**

    -   Completar las clases "VSocket" y "Socket" para poder intercambiar mensajes entre procesos que no comparten memoria. **Para este trabajo deben completar:**

        - Clase "VSocket"

            - ```VSocket::CreateVSocket( int )``` Es el "constructor" de la clase VSocket, recibe el descriptor generado por el llamado al sistema "accept" y construye una nueva instancia

            - ```virtual VSocket * VSocket::Accept() = 0;```

            - ```int VSocket::Listen( int )``` Marca el socket como pasivo, es decir que va a recibir conexiones por medio de Accept, el parámetro establece el tamaño de la cola (backlog), utiliza el llamado al sistema "listen"

            - ```int VSocket::Bind( int )``` Para relacionar este socket con el proceso (bind), el parámetro determina el # de puerto a utilizar. Para establecer que direcciones pueden conectarse a nuestro servidor hay que indicar ese rango en el parámetro "address" utilizando esta estructura para IPv4:

                - ```bash
                    int st;
                    struct sockaddr_in server_addr;
                    ...
                    server_addr.sin_family = AF_INET;	// Definimos la familia para IPv4
                    server_addr.sin_addr.s_addr = htonl( INADDR_ANY );	// Establecemos cualquier dirección
                    server_addr.sin_port = htons( PORT );	// El puerto asociado al servicio
                    ...
                    len = sizeof( server_addr );
                    ...
                    st = bind( id, (const sockaddr *) & server_addr, len );
                    ```

            - ```int VSocket::Shutdown( int )``` Cierra parcialmente el Socket de acuerdo con el parámetro especificado

        - Clase "Socket"
            - ```Socket * Accept()``` Acepta una solicitud de conexión TCP y crea un nuevo "handle" para intercambiar información, para ello deben utilizar el llamado al sistema "accept" y hacer que devuelva una nueva instancia de la clase "Socket"

            - ```Socket::Socket( int )``` otro constructor que recibe un entero como parámetro para construir una instancia en un socket que ya fue creado (abierto), se utiliza en "Accept" para construir y devolver una instancia de la clase "Socket"

    - Los ejemplos provistos `ForkMirrorServer.cc` y `MirrorClient.cc` deben funcionar correctamente

    - También el servidor con hilos `ThreadMirrorServer.cc` debe funcionar correctamente con el cliente `MirrorClient.cc`

   - ## Ejecución IPv4
   - Revisar el codigo para hacer los cambios correspondientes de las IP para que funcione en el equipo que se vaya a utilizar
     - ForkMirrorServer
       - En una terminal
          ```bash
          make ForkMirrorServer.out 
          ```
          ```bash
          ./ForkMirrorServer.out ipv4
          ```
       - En otra terminal distinta
          ```bash
          make MirrorClient.out 
          ```
          Tambien se puede no enviar mensaje, en ese caso se enviará un mensaje predeterminado
          ```bash
          ./MirrorClient.out ipv4 <Mensaje a enviar>
          ```
     - ThreadMirrorServer
       - En una terminal
          ```bash
          make ThreadMirrorServer.out 
          ```
          ```bash
          ./ThreadMirrorServer.out ipv4
          ```
       - En otra terminal distinta
          ```bash
          make MirrorClient.out 
          ```
          Tambien se puede no enviar mensaje, en ese caso se enviará un mensaje predeterminado
          ```bash
          ./MirrorClient.out ipv4 <Mensaje a enviar>
          ```

- ## Notas
	- Si la Address ya está en uso, correr:
		```bash
		sudo lsof -i :5678
		```
		```bash
		sudo kill -9 <PID>
		```
	
# Semana5 - server TCP IPV4 Tarea

  - ## Descripción 

    -   Completar la funcionalidad de servidor para TCP IPv6, tanto para procesos como hilos

   - ## Ejecución IPv6
   - Revisar el codigo para hacer los cambios correspondientes de las IP para que funcione en el equipo que se vaya a utilizar
     - ForkMirrorServer
       - En una terminal
          ```bash
          make ForkMirrorServer.out 
          ```
          ```bash
          ./ForkMirrorServer.out ipv6
          ```
       - En otra terminal distinta
          ```bash
          make MirrorClient.out 
          ```
          Tambien se puede no enviar mensaje, en ese caso se enviará un mensaje predeterminado
          ```bash
          ./MirrorClient.out ipv6 <Mensaje a enviar>
          ```
     - ThreadMirrorServer
       - En una terminal
          ```bash
          make ThreadMirrorServer.out 
          ```
          ```bash
          ./ThreadMirrorServer.out ipv6
          ```
       - En otra terminal distinta
          ```bash
          make MirrorClient.out 
          ```
          Tambien se puede no enviar mensaje, en ese caso se enviará un mensaje predeterminado
          ```bash
          ./MirrorClient.out ipv6 <Mensaje a enviar>
          ```

# Referencias

-   https://os.ecci.ucr.ac.cr/ci0123/Asignaciones/SocketsCourse.ppt