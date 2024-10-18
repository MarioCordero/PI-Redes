# Compiladores y banderas
CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c
LDFLAGS := -lssl -lcrypto   # Añadimos las librerías necesarias de OpenSSL
DEPS := Socket.h SSLSocket.h VSocket.h  # Dependencias necesarias
DEMO_TARGETS := SSLServerDemo.out SSLClientDemo.out  # Ejecutables demo
REAL_TARGETS := SSLServer.out SSLClient.out MirrorClient.out  # Ejecutables reales

ALL := $(DEMO_TARGETS) $(REAL_TARGETS)
$(info $$TARGETS = $(ALL))

# Regla genérica para compilar archivos .c a .o
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

# Regla genérica para compilar archivos .cc a .o
%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

# Reglas para generar ejecutables de demo
SSLServerDemo.out: SSLServer.c VSocket.o Socket.o
	$(CXX) SSLServer.c VSocket.o Socket.o $(LDFLAGS) -o $@

SSLClientDemo.out: SSLClient.c VSocket.o Socket.o
	$(CXX) SSLClient.c VSocket.o Socket.o $(LDFLAGS) -o $@

# Reglas para generar ejecutables reales
SSLServer.out: SSLServer.cc VSocket.o Socket.o SSLSocket.o
	$(CXX) SSLServer.cc VSocket.o Socket.o SSLSocket.o $(LDFLAGS) -o $@

SSLClient.out: SSLClient.cc VSocket.o Socket.o SSLSocket.o
	$(CXX) SSLClient.cc VSocket.o Socket.o SSLSocket.o $(LDFLAGS) -o $@

MirrorClient.out: MirrorClient.cc VSocket.o Socket.o SSLSocket.o
	$(CXX) MirrorClient.cc VSocket.o Socket.o SSLSocket.o $(LDFLAGS) -o $@

# Limpiar los archivos objeto y ejecutables
clean:
	rm -f *.o *.out