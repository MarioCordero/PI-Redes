# Linux Makefile

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c
LDFLAGS := -lssl -lcrypto   # Añadimos las librerías necesarias de OpenSSL
DEPS := Socket.h VSocket.h  # Dependencias necesarias
TARGETS := SSLServerDemo.out SSLClientDemo.out MirrorClient.out

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $< -o $@

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $< -o $@

%.out: %.o VSocket.o Socket.o
	$(info $$^ = $^)
	$(CXX) $^ $(LDFLAGS) -o $@

SSLServerDemo.out: SSLServer.o VSocket.o Socket.o
	$(CXX) SSLServer.o VSocket.o Socket.o $(LDFLAGS) -o $@

SSLClientDemo.out: SSLClient.o VSocket.o Socket.o
	$(CXX) SSLClient.o VSocket.o Socket.o $(LDFLAGS) -o $@

MirrorClient.out: MirrorClient.o VSocket.o Socket.o
	$(CXX) MirrorClient.o VSocket.o Socket.o $(LDFLAGS) -o $@

clean:
	rm -f *.o *.out	