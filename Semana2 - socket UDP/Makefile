# Linux Makefile
# Socket class
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c
DEPS := Socket.h

# Rule to compile source files into object files
%.o: %.cc $(DEPS)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rules for building targets
ipv4-udp-client.out: ipv4-udp-client.o VSocket.o Socket.o
	@echo "Linking ipv4-udp-client.out"
	$(CXX) -g ipv4-udp-client.o VSocket.o Socket.o -o $@

ipv4-udp-server.out: ipv4-udp-server.o VSocket.o Socket.o
	@echo "Linking ipv4-udp-server.out"
	$(CXX) -g ipv4-udp-server.o VSocket.o Socket.o -o $@

ipv6-udp-client.out: ipv6-udp-client.o VSocket.o Socket.o
	@echo "Linking ipv6-udp-client.out"
	$(CXX) -g ipv6-udp-client.o VSocket.o Socket.o -o $@

ipv6-udp-server.out: ipv6-udp-server.o VSocket.o Socket.o
	@echo "Linking ipv6-udp-server.out"
	$(CXX) -g ipv6-udp-server.o VSocket.o Socket.o -o $@

clean:
	@echo "Cleaning up..."
	rm -f *.o *.out

# Usage instructions
#---------------------PARA IPv4---------------------#
#make ipv4-udp-client.out; make ipv4-udp-server.out
#./ipv4-udp-server.out (En una terminal)
#./ipv4-udp-client.out (En otra terminal)

#---------------------PARA IPv6---------------------#
#make ipv6-udp-client.out; make ipv6-udp-server.out
#./ipv6-udp-server.out (En una terminal)
#./ipv6-udp-client.out (En otra terminal)
