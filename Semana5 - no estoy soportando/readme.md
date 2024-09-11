Completar la clase "VSocket" con los métodos necesarios para construir un servidor TCP en IPv4
Métodos a construir
void VSocket::InitVSocket( int ) [construye una instancia utilizando el descriptor pasado como parámetro]
int VSocket::Listen( int )
int VSocket::Bind( int )
virtual VSocket * VSocket::Accept() = 0;
int VSocket::Shutdown ( int )
Completar la clase "Socket" con los métodos necesarios para construir un servidor TCP IPv4
Métodos a construir
Socket::Socket( int ) [construye una instancia utilizando el descriptor pasado como parámetro]
Socket * Socket::Accept ()
Pruebas a realizar
El servidor que crea procesos (fork) debe funcionar correctamente
El servidor que crea hilos (PThreads) debe funcionar correctamente