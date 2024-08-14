# Linux Makefile
# Socket class
# This makefile crates a

CC  := gcc
CFLAGS := -g -c

CXX := g++ #C++ compiler
CXXFLAGS := -g -c #C++ Flags

DEPS := Socket.h #Dependencies
TARGETS := ipv4-test.out #Objetives to build

ALL := $(TARGETS)

$(info $$TARGETS = $(TARGETS)) #Prints a msg during make

%.o: %.cc $(DEPS) #This will make object codes for each of the .cc files
#Compile each file with de C++ flags and the C++ compiler	
	$(CXX) $(CXXFLAGS) $< 


%.out: %.o Socket.o VSocket.o #This makes an executable, yes, just one
	$(info $$^ = $^)
#Prints info in console
	$(CXX) -g $^ Socket.o VSocket.o -o $^
#Makes a executable taking all the ".o" in the scope of this route
#$(CXX) -g $^ -o $@


# <OBJETIVE> : <DEPENDENCIES>
ipv4-test.out: ipv4-test.o Socket.o VSocket.o #Just build an executable for the "ipv4-test.out" file 
	$(CXX) -g ipv4-test.o Socket.o VSocket.o -o $@

ipv6-test.out: ipv6-test.o Socket.o #Just build an executable for the "ipv6-test.out" file 
	$(CXX) -g ipv6-test.o Socket.o VSocket.o -o $@

clean:
	rm -f *.o *.out

#make
#./ipv4-test.out
#make clean