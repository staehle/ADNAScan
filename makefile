CC=g++
MPICC=mpic++
STD=-std=gnu++0x

CFLAGS=-pthread -Wall -Wextra
MPIFLAGS=-Wall -Wextra

SPLITSRC=splitter.cpp
SCANSRC=scanner.cpp

SPLITTARGET=adna-split
SCANTARGET=adna-scan

MPINPTEST=8
SPLITTHREADS=4
FASTQ1=test1.fastq
FASTQ2=test2.fastq

all:
	$(MAKE) clean
	mkdir -p ./bin
	mkdir -p ./raw
	$(CC) $(STD) $(CFLAGS) $(SPLITSRC) -o ./bin/$(SPLITTARGET)
	#$(MPICC) $(STD) $(MPIFLAGS) $(SCANSRC) -o./bin/$(SCANTARGET)
	
clean:
	rm -f ./raw/*
	rm -f *.o
	rm -f $(SPLITTARGET)
	rm -f $(SCANTARGET)

testsplit:
	rm -f ./raw/*
	./bin/$(SPLITTARGET) $(FASTQ1) $(FASTQ2) -t $(SPLITTHREADS) 

testscan:
	mpirun -np ./bin/$(MPINPTEST) $(SCANTARGET)
