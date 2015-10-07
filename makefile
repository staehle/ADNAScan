CC=g++
MPICC=mpic++
STD=-std=gnu++11
CFLAGS=-O3 -Wall -Wextra
SOURCES=splitter.cpp
MPISRCS=scanner.cpp
TARGET=bin/adna-split
MPITARGET=bin/adna-scan
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
