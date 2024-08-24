# Linux Makefile
# Socket class
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c
DEPS := Socket.h
TARGETS_IPV4 := ipv4-udp-client.out ipv4-udp-server.out
TARGETS_IPV6 := ipv6-udp-client.out ipv6-udp-server.out

# Select IPv4 or IPv6
USE_IPV4 := yes  # Change to 'no' to build IPv6 versions

ifeq ($(USE_IPV4), yes)
    TARGETS := $(TARGETS_IPV4)
else
    TARGETS := $(TARGETS_IPV6)
endif

$(info $$TARGETS = $(TARGETS))

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $<

# Rules for IPv4 targets
ipv4-udp-client.out: ipv4-udp-client.o VSocket.o Socket.o
	$(CXX) -g ipv4-udp-client.o VSocket.o Socket.o -o $@

ipv4-udp-server.out: ipv4-udp-server.o VSocket.o Socket.o
	$(CXX) -g ipv4-udp-server.o VSocket.o Socket.o -o $@

# Rules for IPv6 targets
ipv6-udp-client.out: ipv6-udp-client.o VSocket.o Socket.o
	$(CXX) -g ipv6-udp-client.o VSocket.o Socket.o -o $@

ipv6-udp-server.out: ipv6-udp-server.o VSocket.o Socket.o
	$(CXX) -g ipv6-udp-server.o VSocket.o Socket.o -o $@

clean:
	rm -f *.o *.out

#---------------------PARA IPv4---------------------#
#make USE_IPV4=yes
#make ipv4-udp-server.out 
#./ipv4-udp-server.out (In one terminal)
#./ipv4-udp-client.out (In another terminal)

#---------------------PARA IPv6---------------------#
#make USE_IPV4=no
#make ipv6-udp-server.out 
#./ipv6-udp-server.out (In one terminal)
#./ipv6-udp-client.out (In another terminal)