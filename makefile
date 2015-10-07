CC=g++
MPICC=mpic++
STD=-std=gnu++11
CFLAGS=-O3 -Wall -Wextra
SPLITSRC=splitter.cpp
SCANSRC=scanner.cpp
SPLITTARGET=bin/adna-split
SCANTARGET=bin/adna-scan
MPINPTEST=8

all:
	$(MAKE) clean
	$(MPICC) $(STD) $(CFLAGS) $(SPLITSRC) -o $(SPLITTARGET)
	$(MPICC) $(STD) $(CFLAGS) $(SCANSRC) -o $(SCANTARGET)
	
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(MPITARGET)

testsplit:
	mpirun -np $(MPINPTEST) $(SPLITTARGET)

testscan:
	mpirun -np $(MPINPTEST) $(SCANTARGET)
