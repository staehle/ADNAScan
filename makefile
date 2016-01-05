CC=mpic++
BART=/usr/local/gcc-5.3.0/bin/mpic++
STD=-std=gnu++0x
CFLAGS=-Wall -Wextra

SRC=adna.cpp ReadPair.cpp
TARGET=adna

#PROCS=4
#FASTQ1=test1.fastq
#FASTQ2=test2.fastq

all:
	$(MAKE) clean
	mkdir -p ./bin
	$(CC) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGET)
	
clean:
	rm -f *.o
	rm -f ./bin/*

bart:
	$(MAKE) clean
	mkdir -p ./bin
	$(BART) $(STD) $(CFLAGS) $(SRC) -o ./bin/$(TARGET) 

#test:
#	mpirun -np $(PROCS) ./bin/$(TARGET) ./test/$(FASTQ1) ./test/$(FASTQ2) 
