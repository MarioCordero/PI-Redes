# Semana1 - socket

## Descripción

Este proyecto es una implementación de un cliente de red utilizando sockets en C++. El cliente se conecta a un servidor utilizando el protocolo TCP sobre IPv4, envía una solicitud HTTP y recibe una respuesta del servidor. Este proyecto se enfoca en la programación de redes y sistemas operativos, específicamente en el uso de sockets para la comunicación de red.

## Características

- Implementación de un cliente de red en C++.
- Comunicación utilizando sockets TCP sobre IPv4.
- Envío y recepción de datos a través de un socket.
- Ejemplo de solicitud HTTP y manejo de respuestas.

## Requisitos

- **Sistema Operativo**: Linux (Fedora o similar).
- **Compilador**: g++ (GNU C++ Compiler).
- **Bibliotecas**: No se requieren bibliotecas externas adicionales.

## Compilar el Proyecto

   Utiliza el `Makefile` incluido para compilar el proyecto. Ejecuta:

   ```bash
   make
   ```

   Esto generará el archivo ejecutable `ipv4-test.out`.

## Uso

   1. **Ejecutar el Programa**

      Después de compilar el proyecto, puedes ejecutar el cliente con:

      ```bash
      ./ipv4-test.out
      ```

   2. **Configuración**

      - Puedes modificar el archivo `ipv4-test.cc` para cambiar la dirección IP del servidor, el puerto y la solicitud HTTP.

## Estructura del Proyecto

- `ipv4-test.cc`: Código fuente del cliente de red.
- `Socket.h` y `Socket.cpp`: Implementación de la clase `Socket`.
- `VSocket.h`: Implementación de la clase base `VSocket`.
- `Makefile`: Archivo de construcción para compilar el proyecto.