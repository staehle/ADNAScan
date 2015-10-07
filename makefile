CC=g++
MPICC=mpic++
STD=-std=gnu++11
CFLAGS=-O3 -Wall -Wextra
SOURCES=adnascan.cpp splitter.cpp
MPISRCS=scanner.cpp
TARGET=bin/adnascan
MPITARGET=bin/mpiscanner
MPINPTEST=32

all:
	$(MAKE) clean
	$(CC) $(STD) $(CFLAGS) $(SOURCES) -o $(TARGET)
	$(MAKE) mpimake
	
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(MPITARGET)
	
mpimake:
	$(MPICC) $(STD) $(CFLAGS) $(MPISRCS) -o $(MPITARGET)
	
mpitest:
	$(MAKE) mpimake
	mpirun -np $(MPINPTEST) $(MPITARGET)
