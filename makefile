CC=g++
MPICC=mpic++
STD=-std=gnu++0x

CFLAGS=-pthread -Wall -Wextra
MPIFLAGS=-Wall -Wextra

SPLITSRC=splitter.cpp
SCANSRC=scanner.cpp

FINDERTARGET=adna-finder
SCANTARGET=adna-scan

MPINPTEST=8
FINDTHREADS=4
FASTQ1=test1.fastq
FASTQ2=test2.fastq

all:
	$(MAKE) clean
	mkdir -p ./bin
	mkdir -p ./raw
	$(CC) $(STD) $(CFLAGS) $(SPLITSRC) -o ./bin/$(FINDERTARGET)
	#$(MPICC) $(STD) $(MPIFLAGS) $(SCANSRC) -o./bin/$(SCANTARGET)
	
clean:
	rm -f ./raw/*
	rm -f *.o
	rm -f ./bin/$(FINDERTARGET)
	rm -f ./bin/$(SCANTARGET)

test:
	rm -f ./raw/*
	./bin/$(FINDERTARGET) $(FASTQ1) $(FASTQ2) -t $(FINDTHREADS) 

#testscan:
	#mpirun -np ./bin/$(MPINPTEST) $(SCANTARGET)
