# Linux Makefile
# Socket class
# This makefile crates a

CC  := gcc
CFLAGS := -g -c

CXX := g++ #C++ compiler
CXXFLAGS := -g -c #C++ Flags

DEPS := Socket.h #Dependencies

$(info $$TARGETS = $(TARGETS)) #Prints a msg during make

%.o: %.cc $(DEPS)
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@


# <OBJETIVE> : <DEPENDENCIES>
ipv4-test.out: ipv4-test.o Socket.o VSocket.o #Just build an executable for the "ipv4-test.out" file 
	$(CXX) -g ipv4-test.o Socket.o VSocket.o -o $@

ipv6-test.out: ipv6-test.o Socket.o VSocket.o#Just build an executable for the "ipv6-test.out" file 
	$(CXX) -g ipv6-test.o Socket.o VSocket.o -o $@

clean:
	rm -f *.o *.out

#make
#./ipv4-test.out
#make clean

#make
#make ipv6-test.out
#./ipv6-test.out