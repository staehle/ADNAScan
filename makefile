CC=g++
MPICC=mpic++
STD=-std=gnu++11
CFLAGS=-Wall -Wextra
SPLITSRC=splitter.cpp
SCANSRC=scanner.cpp
SPLITTARGET=bin/adna-split
SCANTARGET=bin/adna-scan
MPINPTEST=8

FASTQ=test.fastq

all:
	$(MAKE) clean
	$(CC) $(STD) $(CFLAGS) $(SPLITSRC) -o $(SPLITTARGET)
	$(MPICC) $(STD) $(CFLAGS) $(SCANSRC) -o $(SCANTARGET)
	
clean:
	rm -f *.o
	rm -f $(TARGET)
	rm -f $(MPITARGET)

testsplit:
	$(SPLITTARGET) $(FASTQ)

testscan:
	mpirun -np $(MPINPTEST) $(SCANTARGET)
