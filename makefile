CC=g++
MPICC=mpic++
STD=-std=gnu++0x

CFLAGS=-pthread -Wall -Wextra
MPIFLAGS=-Wall -Wextra

FINDERSRC=finder.cpp
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
	$(CC) $(STD) $(CFLAGS) $(FINDERSRC) -o ./bin/$(FINDERTARGET)
	#$(MPICC) $(STD) $(MPIFLAGS) $(SCANSRC) -o./bin/$(SCANTARGET)
	
clean:
	rm -f *.o
	rm -f ./bin/$(FINDERTARGET)
	rm -f ./bin/$(SCANTARGET)

test:
	./bin/$(FINDERTARGET) $(FASTQ1) $(FASTQ2) -t $(FINDTHREADS) 

mpitest:
	mpirun -np $(MPINPTEST) ./bin/$(SCANTARGET)
